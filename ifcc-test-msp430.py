#!/usr/bin/env python3
import subprocess
import sys
import os
import re
import argparse
import textwrap
import shutil

# Chemins
PLD_BASE_DIR = os.path.normpath(os.path.dirname(__file__))
COMPILER = f"{PLD_BASE_DIR}/compiler/ifcc"
MSP430_AS = "msp430-elf-as"
MSP430_LD = "msp430-elf-ld"
LINKER_SCRIPT = "/mnt/c/msp430_emul/mspsim/msp430-gcc-9.3.1.11_linux64/msp430-elf/lib/430/exceptions/msp430xl-sim.ld"
CRT0 = "/mnt/c/msp430_emul/mspsim/msp430-gcc-9.3.1.11_linux64/msp430-elf/lib/430/crt0.o"
WORK_DIR = "/tmp/ifcc-msp430-test"
MSP430_TOOLCHAIN = "/mnt/c/msp430_emul/mspsim/msp430-gcc-9.3.1.11_linux64/bin"

def run_command(string, logfile=None, toscreen=False):
    if args.debug:
        print("ifcc-test-msp430.py: " + string)

    process = subprocess.Popen(string, shell=True,
                               stderr=subprocess.STDOUT, stdout=subprocess.PIPE,
                               text=True, bufsize=0)
    if logfile:
        logfile = open(logfile, 'w')

    while True:
        output = process.stdout.readline()
        if len(output) == 0:
            break
        if logfile: logfile.write(output)
        if toscreen: sys.stdout.write(output)
    process.wait()
    assert process.returncode != None
    if logfile:
        logfile.write(f'\nexit status: {process.returncode}\n')
    return process.returncode

def run_subprocess(cmd):
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    return result.returncode, result.stdout, result.stderr

def dumpfile(name, quiet=False):
    try:
        data = open(name, "rb").read().decode('utf-8', errors='ignore')
        if not quiet:
            print(data, end='')
        return data
    except:
        return ""

def assemble_and_link(asm_file, elf_file, logfile=None):
    obj_file = elf_file.replace(".elf", ".o")
    rc = run_command(f"{MSP430_AS} -mmcu=msp430f1611 {asm_file} -o {obj_file}", logfile)
    if rc != 0:
        return rc
    rc = run_command(f"{MSP430_LD} -T {LINKER_SCRIPT} {CRT0} {obj_file} -o {elf_file}", logfile)
    return rc

def get_ret_address(elf_file):
    """Trouve l'adresse de pop r4 juste avant ret dans main"""
    rc, stdout, _ = run_subprocess(f"msp430-elf-objdump -d {elf_file}")
    match = re.search(r'^\s+([0-9a-fA-F]+):\s+34 41\s+pop\s+r4', stdout, re.MULTILINE)
    if match:
        return int(match.group(1), 16)
    return None

def get_main_address(elf_file):
    """Trouve l'adresse de main dans le .elf"""
    rc, stdout, _ = run_subprocess(f"msp430-elf-nm {elf_file}")
    match = re.search(r'^([0-9a-fA-F]+)\s+T\s+main$', stdout, re.MULTILINE)
    if match:
        addr = int(match.group(1), 16)
        low  = addr & 0xFF
        high = (addr >> 8) & 0xFF
        return low, high
    return 0x00, 0x05  # fallback

def run_in_mspdebug(elf_file):
    ret_addr = get_ret_address(elf_file)
    if ret_addr is None:
        if args.debug:
            print("  [debug] impossible de trouver pop r4")
        return None

    low, high = get_main_address(elf_file)

    if args.debug >= 1:
        print(f"  [debug] main=0x{high:02x}{low:02x}, breakpoint pop r4 à 0x{ret_addr:04x}")

    script = (
        f"prog {elf_file}\n"
        f"mw 0xfffe 0x{low:02x}\n"
        f"mw 0xffff 0x{high:02x}\n"
        f"reset\n"
        f"setbreak 0x{ret_addr:04x}\n"
        f"run\n"
        f"printreg\n"
        f"quit\n"
    )
    process = subprocess.Popen(
        "mspdebug sim 2>/dev/null", shell=True,
        stdin=subprocess.PIPE, stdout=subprocess.PIPE,
        text=True
    )
    try:
        stdout, _ = process.communicate(input=script, timeout=15)
    except subprocess.TimeoutExpired:
        process.kill()
        process.communicate()
        return None

    if args.debug >= 2:
        print(f"  [debug] mspdebug stdout: {stdout}")

    match = re.search(r'R15:\s*([0-9a-fA-F]+)', stdout)
    if match:
        return int(match.group(1), 16)
    return None

######################################################################################
## ARGPARSE

orig_cwd = os.getcwd()

width = shutil.get_terminal_size().columns - 2
twf = lambda text: textwrap.fill(text, width, initial_indent='    ', subsequent_indent='      ')

argparser = argparse.ArgumentParser(
    formatter_class=argparse.RawDescriptionHelpFormatter,
    description="Testing script for the ifcc MSP430 backend.\n\n"
        + twf("Compile each test-case with ifcc --msp430, assemble with msp430-elf-as, "
              "run in mspdebug sim, and compare R15 with the gcc x86 exit code."),
    epilog="examples:\n\n"
        + twf("python3 ifcc-test-msp430.py testfiles/") + '\n'
        + twf("python3 ifcc-test-msp430.py testfiles/ -v") + '\n'
        + twf("python3 ifcc-test-msp430.py testfiles/1_return42.c -vv"),
)

argparser.add_argument('input', metavar='PATH', nargs='+',
                       help="For each path given: if it's a file, use this file; "
                            "if it's a directory, use all *.c files under this subtree")
argparser.add_argument('-v', '--verbose', action='count', default=0,
                       help='increase verbosity level. You can use this option multiple times.')
argparser.add_argument('-d', '--debug', action='count', default=0,
                       help='increase quantity of debugging messages')
argparser.add_argument('--keep', action='store_true',
                       help='keep temporary files after tests')

args = argparser.parse_args()

if args.debug >= 2:
    print('debug: command-line arguments ' + str(args))

######################################################################################
## PREPARE step

os.environ["PATH"] = f"{MSP430_TOOLCHAIN}:" + os.environ["PATH"]

makestatus = run_command(f'cd {PLD_BASE_DIR}/compiler; make --question ifcc')
if makestatus:
    makestatus = run_command(f'cd {PLD_BASE_DIR}/compiler; make ifcc', toscreen=True)
    if makestatus:
        if os.path.exists("ifcc"):
            os.unlink("ifcc")
        exit(makestatus)

os.chdir(orig_cwd)
inputfilenames = []
for path in args.input:
    path = os.path.normpath(path)
    if os.path.isfile(path):
        if path[-2:] == '.c':
            inputfilenames.append(path)
        else:
            print("error: incorrect filename suffix (should be '.c'): " + path)
            exit(1)
    elif os.path.isdir(path):
        for dirpath, dirnames, filenames in os.walk(path):
            inputfilenames += [dirpath + '/' + name for name in filenames if name[-2:] == '.c']
    else:
        print("error: cannot read input path `" + path + "'")
        sys.exit(1)

inputfilenames = sorted(inputfilenames)

if args.debug:
    print("debug: list of files after tree walk:", " ".join(inputfilenames))

if len(inputfilenames) == 0:
    print("error: found no test-case in: " + " ".join(args.input))
    sys.exit(1)

if os.path.isdir(WORK_DIR):
    shutil.rmtree(WORK_DIR)
os.makedirs(WORK_DIR)

######################################################################################
## TEST step

all_ok = True
nb_ok = 0
nb_fail = 0
nb_skip = 0

for c_file in inputfilenames:
    print('TEST-CASE: ' + c_file)
    os.chdir(WORK_DIR)

    c_file_abs = os.path.abspath(os.path.join(orig_cwd, c_file))
    basename = os.path.basename(c_file).replace(".c", "")

    asm_ifcc = f"{WORK_DIR}/{basename}_ifcc.s"
    elf_ifcc = f"{WORK_DIR}/{basename}_ifcc.elf"
    ref_exe  = f"{WORK_DIR}/{basename}_ref"

    # Reference compiler = GCC x86
    gccstatus = run_command(f"gcc -o {ref_exe} {c_file_abs}",
                            f"{WORK_DIR}/{basename}_gcc-compile.txt")
    if gccstatus == 0:
        exegccstatus = run_command(f"{ref_exe}",
                                   f"{WORK_DIR}/{basename}_gcc-execute.txt")
        ref_value = exegccstatus & 0xFF
        if args.verbose >= 2:
            dumpfile(f"{WORK_DIR}/{basename}_gcc-execute.txt")

    # IFCC compiler --msp430
    ifccstatus = run_command(
        f"{COMPILER} --msp430 {c_file_abs} > {asm_ifcc}",
        f"{WORK_DIR}/{basename}_ifcc-compile.txt"
    )

    # cas : mul/div non supporté → SKIP
    if ifccstatus != 0:
        compile_log = dumpfile(f"{WORK_DIR}/{basename}_ifcc-compile.txt", quiet=True)
        if "multiplication non supportée" in compile_log or "division non supportée" in compile_log:
            print("  SKIP (mul/div non supporté sur MSP430)")
            nb_skip += 1
            continue

    # cas : les deux rejettent → OK
    if gccstatus != 0 and ifccstatus != 0:
        print("TEST OK")
        nb_ok += 1
        continue

    # cas : gcc rejette, ifcc accepte → FAIL
    elif gccstatus != 0 and ifccstatus == 0:
        print("TEST FAIL (your compiler accepts an invalid program)")
        all_ok = False
        nb_fail += 1
        if args.verbose:
            dumpfile(asm_ifcc)
            dumpfile(f"{WORK_DIR}/{basename}_ifcc-compile.txt")
        continue

    # cas : gcc accepte, ifcc rejette → FAIL
    elif gccstatus == 0 and ifccstatus != 0:
        print("TEST FAIL (your compiler rejects a valid program)")
        all_ok = False
        nb_fail += 1
        if args.verbose:
            dumpfile(asm_ifcc)
            dumpfile(f"{WORK_DIR}/{basename}_ifcc-compile.txt")
        continue

    # cas : les deux acceptent → assembler, linker, simuler
    ldstatus = assemble_and_link(asm_ifcc, elf_ifcc,
                                  f"{WORK_DIR}/{basename}_ifcc-link.txt")
    if ldstatus:
        print("TEST FAIL (your compiler produces incorrect assembly)")
        all_ok = False
        nb_fail += 1
        if args.verbose:
            dumpfile(asm_ifcc)
            dumpfile(f"{WORK_DIR}/{basename}_ifcc-link.txt")
        continue

    # simuler avec mspdebug
    r15 = run_in_mspdebug(elf_ifcc)
    if r15 is None:
        print("TEST FAIL (cannot read R15 from mspdebug)")
        all_ok = False
        nb_fail += 1
        if args.verbose:
            dumpfile(asm_ifcc)
        continue

    r15_masked = r15 & 0xFF
    if r15_masked != ref_value:
        print("TEST FAIL (different results at execution)")
        all_ok = False
        nb_fail += 1
        if args.verbose:
            print(f"GCC: {ref_value}")
            print(f"you: {r15_masked} (R15=0x{r15:04x})")
        continue

    print("TEST OK")
    nb_ok += 1

# Nettoyage
if not args.keep and os.path.isdir(WORK_DIR):
    shutil.rmtree(WORK_DIR)

######################################################################################
## RÉSULTATS

nb_total = nb_ok + nb_fail
print(f"\n{'='*40}")
print(f"RÉSULTATS : {nb_ok}/{nb_total} tests validés ({nb_skip} skippés)", end="")
if nb_fail > 0:
    print(f" ({nb_fail} échec(s))")
else:
    print(" -- Tous les tests sont OK !")
print('='*40)

if not (all_ok or args.verbose):
    print("Some test-cases failed. Run ifcc-test-msp430.py with option '--verbose' for more detailed feedback.")

sys.exit(0 if all_ok else 1)