# Compilateur IFCC — Compilateur C

> **INSA de Lyon — 4IF — PLD-Comp — Année 2025-2026**  
> **Etudiants** : Ines Souibgui, Yassine Taharaste, Bilal Kaya, Hiba Ben Amor, Ikram Iqdari, Nour El Ouardighi, Malak Bahda
> **Lien github** : https://github.com/RED-ICE-95/PLD_COMP.git

## Compilation et exécution

### Prérequis

- ANTLR4 et son runtime C++ installés (voir section 4.1 du sujet)
- GCC (g++) pour la compilation du compilateur et de l'assembleur produit
- Python 3 pour les scripts de tests
- *(MSP430 uniquement)* Toolchain MSP430 : `msp430-elf-as`, `msp430-elf-ld`, `mspdebug`

### Construire le compilateur

```bash
cd compiler
make
```

L'exécutable `compiler/ifcc` est produit.

### Utilisation

**Cible x86-64 (par défaut) :**
```bash
./compiler/ifcc monprogramme.c > monprogramme.s
gcc monprogramme.s -o monprogramme
./monprogramme
echo $?
```

**Cible MSP430 :**
```bash
./compiler/ifcc --msp430 monprogramme.c > monprogramme.s
```

### Lancer les tests

**Tests x86-64 :**
```bash
python3 ifcc-test.py testfiles/
```

**Tests MSP430 :**
```bash
python3 ifcc-test-msp430.py testfiles/ --verbose
```

L'option `--help` affiche toutes les options disponibles.

---

## 1. Fonctionnalités implémentées

### Architecture du compilateur

Le compilateur suit un pipeline classique en trois étapes :

```
Source C  →  [ANTLR4 Lexer/Parser]  →  [SymbolTableVisitor]  →  [CodeGenVisitor → IR → CFG]  →  Assembleur (x86-64 ou MSP430)
```

1. **Analyse lexicale et syntaxique** : assurée par ANTLR4 à partir de la grammaire [compiler/ifcc.g4](compiler/ifcc.g4).
2. **Analyse sémantique** : effectuée par [SymbolTableVisitor](compiler/SymbolTableVisitor.cpp) (premier passage sur l'AST).
3. **Génération de code** : effectuée par [CodeGenVisitor](compiler/CodeGenVisitor.cpp) qui construit une IR (BasicBlocks + CFG), puis traduit en assembleur pour la cible choisie.

---

### Fonctionnalités du langage supportées

#### Types
| Type | Support |
|------|---------|
| `int` (32 bits x86 / 16 bits MSP430) | ok |
| Constante `char` (valeur ASCII) | ok |

#### Instructions
| Construction | Exemple | Fichier de test |
|---|---|---|
| Entier littéral | `return 42;` | [1_return42.c](testfiles/1_return42.c) |
| Déclaration de variable | `int a;` | [3_return_var.c](testfiles/3_return_var.c) |
| Déclaration multiple | `int a, b, c;` | [21_multi_var_chain.c](testfiles/21_multi_var_chain.c) |
| Initialisation à la déclaration | `int a = expr;` | [71_init_simple.c](testfiles/tests_declaration_affectation_combine/71_init_simple.c) |
| Affectation | `a = expr;` | [3_return_var.c](testfiles/3_return_var.c) |
| Instruction `return` (n'importe où) | `return expr;` | [1_return42.c](testfiles/1_return42.c) |
| Bloc `{ }` avec portée lexicale | `{ int x = 1; }` | [tests_bloc/](testfiles/tests_bloc/) |
| `if` / `else` | `if (a > 0) { ... }` | [tests_conditionnelles/](testfiles/tests_conditionnelles/) |
| `while` | `while (i > 0) { ... }` | [tests_conditionnelles/](testfiles/tests_conditionnelles/) |
| Appel de fonction | `f(a, b)` | [tests_function/](testfiles/tests_function/) |
| `putchar` | `putchar('A');` | [tests_putchar_getchar/](testfiles/tests_putchar_getchar/) |
| `getchar` | `int c = getchar();` | [tests_putchar_getchar/](testfiles/tests_putchar_getchar/) |

#### Opérateurs arithmétiques
| Opérateur | Description | Fichier de test |
|---|---|---|
| `+`, `-` | Addition, soustraction | [6_left_assoc_sub.c](testfiles/6_left_assoc_sub.c) |
| `*` | Multiplication | [5_mul_priority.c](testfiles/5_mul_priority.c) |
| `/` | Division entière | [8_div_mod.c](testfiles/8_div_mod.c) |
| `%` | Modulo | [8_div_mod.c](testfiles/8_div_mod.c) |
| `-expr` | Négation unaire | [9_unary.c](testfiles/9_unary.c) |
| `!expr` | NOT logique | [9_unary.c](testfiles/9_unary.c) |

#### Constantes caractère

Les constantes de type `char` sont supportées comme des entiers (valeur ASCII). Elles peuvent être utilisées dans n'importe quelle expression à la place d'un entier.

| Forme | Exemple | Valeur | Fichier de test |
|---|---|---|---|
| Caractère simple | `'A'` | 65 | [12_char_const.c](testfiles/12_char_const.c) |
| Séquence d'échappement `\n` | `'\n'` | 10 | [13_char_escape.c](testfiles/13_char_escape.c) |
| Séquence d'échappement `\t` | `'\t'` | 9 | [13_char_escape.c](testfiles/13_char_escape.c) |
| Char dans une expression | `'A' + 5` | 70 | [20_char_plus_int.c](testfiles/20_char_plus_int.c) |
| Arithmétique entre chars | `'Z' - 'A'` | 25 | [12_char_const.c](testfiles/12_char_const.c) |

Les séquences d'échappement reconnues sont : `\n`, `\t`, `\r`, `\0`, `\\`, `\'`, `\"`.

---

#### Opérateurs bit-à-bit
| Opérateur | Description | Fichier de test |
|---|---|---|
| `&` | ET bit-à-bit | [10_bitwise.c](testfiles/10_bitwise.c) |
| `^` | XOR bit-à-bit | [10_bitwise.c](testfiles/10_bitwise.c) |
| `\|` | OU bit-à-bit | [10_bitwise.c](testfiles/10_bitwise.c) |

#### Opérateurs de comparaison et d'égalité
| Opérateur | Description | Fichier de test |
|---|---|---|
| `==`, `!=` | Égalité / Inégalité | [14_equality.c](testfiles/14_equality.c) |
| `<`, `>` | Inférieur / Supérieur strict | [11_comparisons.c](testfiles/11_comparisons.c) |

#### Priorités et associativité
La grammaire encode directement la hiérarchie des priorités (du moins prioritaire au plus prioritaire) :

```
|  <  ^  <  &  <  ==,!=  <  <,>  <  +,-  <  *,/,%  <  unaires  <  (…)
```

Les tests [5_mul_priority.c](testfiles/5_mul_priority.c), [7_parens_priority.c](testfiles/7_parens_priority.c), [10_bitwise.c](testfiles/10_bitwise.c) et [16_left_assoc_div.c](testfiles/16_left_assoc_div.c) vérifient ces priorités et l'associativité gauche.

---

### Optimisations (propagation de constantes)

Le compilateur évalue les expressions constantes **à la compilation**, sans émettre d'instructions inutiles :

| Expression | Résultat produit |
|---|---|
| `3 + 4` | `$7` (calculé à la compilation) |
| `x + 0`, `0 + x` | `x` (addition neutre) |
| `x - 0` | `x` |
| `x * 1`, `1 * x` | `x` (multiplication neutre) |
| `x * 0`, `0 * x` | `$0` |
| `x / 1` | `x` |
| `x % 1` | `$0` |
| `x ^ x`, `x - x` | `$0` |

---

### Analyse sémantique (table des symboles)

Le [`SymbolTableVisitor`](compiler/SymbolTableVisitor.cpp) effectue, en un seul passage, les vérifications suivantes :

| Erreur / Avertissement | Message | Fichier de test |
|---|---|---|
| Double déclaration | `Erreur : variable 'x' déjà déclarée.` | [23_invalid_double_decl.c](testfiles/23_invalid_double_decl.c) |
| Variable non déclarée à droite d'une expression | `Erreur : variable 'x' utilisée sans déclaration.` | [24_invalid_undecl_rhs.c](testfiles/24_invalid_undecl_rhs.c) |
| Variable non déclarée à gauche d'une affectation | `Erreur : variable 'x' utilisée sans déclaration.` | [25_invalid_undecl_lhs.c](testfiles/25_invalid_undecl_lhs.c) |
| Variable déclarée mais jamais utilisée | `Avertissement : variable 'x' déclarée mais jamais utilisée.` | [26_unused_var_warning.c](testfiles/26_unused_var_warning.c) |
| Appel de fonction non déclarée | `Erreur : appel à la fonction 'foo' non déclarée.` | [tests_function/](testfiles/tests_function/) |
| Nombre d'arguments incorrect | `Erreur : nombre incorrect d'arguments` | [tests_function/](testfiles/tests_function/) |
| Utilisation de `void` dans une expression | `Erreur : impossible d'utiliser void dans une expression` | [tests_function/](testfiles/tests_function/) |
| Erreur de syntaxe | `error: syntax error during parsing` | [2_invalid_program.c](testfiles/2_invalid_program.c) |

---

### Backend MSP430

Le compilateur supporte une deuxième cible : le microcontrôleur **MSP430** (16 bits).

| Aspect | x86-64 | MSP430 |
|---|---|---|
| Registres | `%rbp`, `%rsp`, `%eax` | R4 (FP), R1 (SP), R15 (retour) |
| Variables sur pile | `-offset(%rbp)` | `-offset(R4)` |
| Taille variable | 4 octets | 2 octets |
| Passage de paramètres | `%edi`, `%esi`, `%edx`, ... | R12, R13, R14 |
| Appel de fonction | `call f@PLT` | `call #f` |
| Limitations | — | Pas de `*`, `/`, `%` matériel |

Le script `ifcc-test-msp430.py` compile, assemble, linke et exécute dans le simulateur `mspdebug` en comparant R15 avec le code de retour GCC x86.

---

### Génération de code x86-64 (AT&T)

Le [`CodeGenVisitor`](compiler/CodeGenVisitor.cpp) produit de l'assembleur x86-64 en syntaxe AT&T, compatible Linux (et macOS via `#ifdef __APPLE__`).

- **Représentation intermédiaire (IR)** : le CodeGenVisitor construit un graphe de BasicBlocks (CFG). Chaque BasicBlock contient une liste d'instructions IR simples (`ldconst`, `copy`, `add`, `sub`, etc.). La traduction en assembleur est faite en un second temps par les méthodes `gen_asm()` de l'IR.
- **Gestion des variables** : chaque variable est allouée sur la pile. L'offset de chaque variable par rapport à `%rbp` est calculé par le CFG et traduit en `-offset(%rbp)`.
- **Convention d'appel** : prologue/épilogue standard (`push %rbp` / `mov %rsp, %rbp` / `leave` / `ret`).
- **Résultat** : la valeur de retour est placée dans `%eax` (registre de retour x86-64).

---

## 2. Navigation dans le code

### Structure des fichiers

```
c:\PLD_COMP\
├── compiler/
│   ├── ifcc.g4                  # Grammaire ANTLR4 du langage
│   ├── main.cpp                 # Point d'entrée : parsing, STV puis CGV
│   ├── type.h                   # Enum des types (INT32, CHAR, VOID)
│   ├── IR.h                     # Classes IRInstr, BasicBlock, CFG
│   ├── IR.cpp                   # Implémentation IR + gen_asm x86 et MSP430
│   ├── CFG_MSP430.h             # Sous-classe CFG pour MSP430
│   ├── CFG_MSP430.cpp           # Prologue/épilogue/codegen MSP430
│   ├── SymbolTableVisitor.h     # Interface de l'analyse sémantique
│   ├── SymbolTableVisitor.cpp   # Vérification des déclarations / usages
│   ├── CodeGenVisitor.h         # Interface de la génération de code
│   ├── CodeGenVisitor.cpp       # Construction de l'IR + gestion des CFG
│   ├── Makefile                 # Build du compilateur
│   ├── generated/               # Code C++ généré par ANTLR4 (ne pas modifier)
│   │   ├── ifccLexer.{h,cpp}
│   │   ├── ifccParser.{h,cpp}
│   │   ├── ifccBaseVisitor.{h,cpp}
│   │   └── ifccVisitor.{h,cpp}
│   └── build/                   # Fichiers de dépendances (.d)
├── testfiles/
│   ├── tests_expressions/       # 70 tests d'opérateurs et expressions
│   ├── tests_return/            # return depuis divers contextes
│   ├── tests_bloc/              # Portées imbriquées
│   ├── tests_conditionnelles/   # if/else et while
│   ├── tests_function/          # Fonctions, paramètres, void
│   ├── tests_putchar_getchar/   # putchar, getchar
│   └── tests_declaration_affectation_combine/  # Init, folding, portées
├── ifcc-test.py                 # Script de test automatisé x86-64
├── ifcc-test-msp430.py          # Script de test automatisé MSP430
└── README.md
```

### Points d'entrée clés dans le code

| Fonctionnalité | Fichier | Emplacement |
|---|---|---|
| Grammaire complète | [compiler/ifcc.g4](compiler/ifcc.g4) | L.1–55 |
| Main / pipeline complet | [compiler/main.cpp](compiler/main.cpp) | L.1–70 |
| Construction de la table des symboles | [compiler/SymbolTableVisitor.cpp](compiler/SymbolTableVisitor.cpp) | `visitDeclar` |
| Vérification des usages | [compiler/SymbolTableVisitor.cpp](compiler/SymbolTableVisitor.cpp) | `visitExprId` |
| Avertissement variables inutilisées | [compiler/SymbolTableVisitor.cpp](compiler/SymbolTableVisitor.cpp) | `visitProg` |
| Génération IR + prologue/épilogue | [compiler/CodeGenVisitor.cpp](compiler/CodeGenVisitor.cpp) | `visitProg` |
| Déclaration de fonction | [compiler/CodeGenVisitor.cpp](compiler/CodeGenVisitor.cpp) | `visitFonctDecl` |
| if/else (BasicBlocks) | [compiler/CodeGenVisitor.cpp](compiler/CodeGenVisitor.cpp) | `visitIf_stmt` |
| while (BasicBlocks + boucle) | [compiler/CodeGenVisitor.cpp](compiler/CodeGenVisitor.cpp) | `visitWhile_stmt` |
| Appel de fonction | [compiler/CodeGenVisitor.cpp](compiler/CodeGenVisitor.cpp) | `visitExprFonctCall` |
| Propagation de constantes | [compiler/CodeGenVisitor.cpp](compiler/CodeGenVisitor.cpp) | `visitExprAdd`, `visitExprMult` |
| Instructions IR (x86) | [compiler/IR.cpp](compiler/IR.cpp) | `IRInstr::gen_asm` |
| Instructions IR (MSP430) | [compiler/IR.cpp](compiler/IR.cpp) | `IRInstr::gen_asm_msp430` |
| Prologue/épilogue MSP430 | [compiler/CFG_MSP430.cpp](compiler/CFG_MSP430.cpp) | `CFG_MSP430::gen_asm` |

---

## 3. Gestion de projet

### Méthodologie agile

Le projet est développé en sprints courts, chacun produisant un compilateur fonctionnel de bout en bout sur un sous-ensemble bien défini du C. Chaque sprint est précédé par l'écriture de tests (approche TDD) qui vérifient les nouvelles fonctionnalités.

### Sprints réalisés

| Sprint | Fonctionnalités ajoutées | Tests associés |
|---|---|---|
| 1 | Installation ANTLR4, build Makefile, grammaire minimale, pipeline complet | — |
| 2 | `return CONST` ; prologue/épilogue x86-64 ; génération assembleur AT&T | [1_return42.c](testfiles/1_return42.c) |
| 3 | Prise en main de `ifcc-test.py` ; ajout de programmes invalides | [2_invalid_program.c](testfiles/2_invalid_program.c), [4_invalid_undecl_var.c](testfiles/4_invalid_undecl_var.c) |
| 4 | Déclaration `int` (simple et multiple) ; affectation ; allocation sur la pile | [3_return_var.c](testfiles/3_return_var.c), [21_multi_var_chain.c](testfiles/21_multi_var_chain.c) |
| 5 | `SymbolTableVisitor` : double déclaration, variable non déclarée, variable inutilisée | [23](testfiles/23_invalid_double_decl.c), [24](testfiles/24_invalid_undecl_rhs.c), [25](testfiles/25_invalid_undecl_lhs.c), [26](testfiles/26_unused_var_warning.c) |
| 6 | Opérations `+`, `-`, `*`, `/`, `%` ; unaires `-`, `!` ; bit-à-bit ; comparaisons ; priorités ; constantes char | [5](testfiles/5_mul_priority.c) à [22](testfiles/22_cmp_in_arith.c) |
| 7 | Représentation intermédiaire (IR) : BasicBlocks, CFG, instructions IR, gen_asm | [tests_expressions/](testfiles/tests_expressions/) |
| 8 | `if` / `else` (y compris imbriqués et chaînés) ; `while` | [tests_conditionnelles/](testfiles/tests_conditionnelles/) |
| 9 | `return` depuis n'importe où ; blocs `{ }` avec portée lexicale | [tests_return/](testfiles/tests_return/), [tests_bloc/](testfiles/tests_bloc/) |
| 10 | Fonctions (`int f(int a, int b)`, `void f()`) ; passage de paramètres ; `putchar` ; `getchar` | [tests_function/](testfiles/tests_function/), [tests_putchar_getchar/](testfiles/tests_putchar_getchar/) |
| 11 | Initialisation à la déclaration ; propagation de constantes ; élimination des neutres | [tests_declaration_affectation_combine/](testfiles/tests_declaration_affectation_combine/) |
| 12 | Backend MSP430 : `CFG_MSP430`, prologue/épilogue, `call #f`, R12/R13/R14, R15, mspdebug | [ifcc-test-msp430.py](ifcc-test-msp430.py) |

### État des tests

Sur les fichiers de test présents dans `testfiles/` :

- **Programmes valides** : la sortie de ifcc (code de retour ou affichage) correspond à celle de gcc.
- **Programmes invalides** : ifcc et gcc échouent tous les deux à compiler → tests considérés comme réussis.
- **Tests MSP430** : exécutés dans le simulateur `mspdebug` ; R15 comparé au code de retour GCC x86.

### Source
Ce fichier README.md a été rédigé à l'aide d'une IA (Claude Claude 4.6).
