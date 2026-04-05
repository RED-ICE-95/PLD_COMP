# Compilateur IFCC — Compilateur C

> **INSA de Lyon — 4IF — PLD-Comp — Année 2025-2026**  
> **Groupe** : H4212
> **Étudiants** : Ines Souibgui, Yassine Taharaste, Bilal Kaya, Hiba Ben Amor, Ikram Iqdari, Nour El Ouardighi, Malak Bahda  
> **Lien github** : https://github.com/RED-ICE-95/PLD_COMP.git

---

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

Les scripts de tests doivent être lancés **depuis la racine du projet** (le dossier contenant `ifcc-test.py`).

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

## Fonctionnalités implémentées

### Architecture du compilateur

Le compilateur suit un pipeline classique en trois étapes :

```
Source C  →  [ANTLR4 Lexer/Parser]  →  [SymbolTableVisitor]  →  [CodeGenVisitor → IR → CFG]  →  Assembleur (x86-64 ou MSP430)
```

1. **Analyse lexicale et syntaxique** : assurée par ANTLR4 à partir de la grammaire [compiler/ifcc.g4](compiler/ifcc.g4).
2. **Analyse sémantique** : effectuée par [SymbolTableVisitor](compiler/SymbolTableVisitor.cpp) (premier passage sur l'AST).
3. **Génération de code** : effectuée par [CodeGenVisitor](compiler/CodeGenVisitor.cpp) qui construit une IR (BasicBlocks + CFG), puis traduit en assembleur pour la cible choisie.

---

### Instructions implémentées

| Construction | Exemple |
|---|---|
| Type de base `int` (32 bits) | `int a;` |
| Variables | `int a;` |
| Constantes entières | `42` |
| Constantes caractère (simple quote) utilisables dans les expressions | `'A'`, `'A' + 5`, `'Z' - 'A'` |
| Affectation | `a = expr;` |
| Initialisation à la déclaration | `int a = expr;` |
| Déclaration multiple | `int a, b, c;` |
| Opérations arithmétiques : `+`, `-`, `*` | `a + b * c` |
| Division et modulo | `a / b`, `a % b` |
| Opérations logiques bit-à-bit : `\|`, `&`, `^` | `a & b` |
| Opérations de comparaison : `==`, `!=`, `<`, `>` | `a < b` |
| Opérateurs logiques paresseux `\|\|`, `&&` | `a && b` |
| Opérations unaires : `!` et `-` | `!a`, `-b` |
| Opérateurs d'affectation `+=`, `-=` etc., d'incrémentation `++` et décrémentation `--` | `a++`, `b += 2` |
| Déclaration de variables n'importe où dans un bloc | — |
| Structure de blocs `{ }` avec portée lexicale et *shadowing* | `{ int x = 1; }` |
| Structures de contrôle `if`, `else`, `while` | `if (a > 0) { ... }` |
| `break` et `continue` | — |
| `switch...case` | `switch(a) { case 1: ... }` |
| `return` n'importe où | `return expr;` |
| Définition de fonctions avec paramètres, type de retour `int` ou `void` | `int f(int a, int b)` |
| Vérification de la cohérence des appels de fonctions et leurs paramètres | — |
| Tableaux à une dimension | `int tab[10];` |
| `putchar` pour écrire un caractère sur la sortie standard | `putchar('A');` |
| `getchar` pour lire un caractère depuis l'entrée standard | `int c = getchar();` |

> **Note sur les constantes caractère** : les littéraux comme `'A'` sont supportés en tant que valeurs entières (valeur ASCII). Ils peuvent être utilisés dans toute expression à la place d'un entier. En revanche, la déclaration `char a;` n'est **pas** supportée, seul `int` est accepté comme type de variable.

> **Note sur `putchar` et `getchar`** : ces fonctions ne sont pas supportées sur la cible MSP430 (absence d'appel système dans le simulateur `mspdebug`).

#### Séquences d'échappement dans les constantes caractère

Les séquences d'échappement reconnues sont : `\n`, `\t`, `\r`, `\0`, `\\`, `\'`, `\"`.

#### Priorités et associativité

La grammaire encode directement la hiérarchie des priorités (du moins prioritaire au plus prioritaire) :

```
|  <  ^  <  &  <  ==,!=  <  <,>  <  +,-  <  *,/,%  <  unaires  <  (…)
```

---

### Optimisations (propagation de constantes)

Le compilateur évalue les expressions constantes **à la compilation** :

| Expression | Résultat produit |
|---|---|
| `3 + 4` | `$7` (calculé à la compilation) |
| `x + 0`, `0 + x` | `x` |
| `x - 0` | `x` |
| `x * 1`, `1 * x` | `x` |
| `x * 0`, `0 * x` | `$0` |
| `x / 1` | `x` |
| `x % 1` | `$0` |
| `x ^ x`, `x - x` | `$0` |

---

### Analyse sémantique (table des symboles)

Le [`SymbolTableVisitor`](compiler/SymbolTableVisitor.cpp) effectue, en un seul passage, les vérifications suivantes :

| Erreur / Avertissement | Message |
|---|---|
| Double déclaration | `Erreur : variable 'x' déjà déclarée.` |
| Variable non déclarée (usage) | `Erreur : variable 'x' utilisée sans déclaration.` |
| Variable déclarée mais jamais utilisée | `Avertissement : variable 'x' déclarée mais jamais utilisée.` |
| Appel de fonction non déclarée | `Erreur : appel à la fonction 'foo' non déclarée.` |
| Nombre d'arguments incorrect | `Erreur : nombre incorrect d'arguments` |
| Utilisation de `void` dans une expression | `Erreur : impossible d'utiliser void dans une expression` |
| Erreur de syntaxe | `error: syntax error during parsing` |

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

- **Représentation intermédiaire (IR)** : le CodeGenVisitor construit un graphe de BasicBlocks (CFG). Chaque BasicBlock contient une liste d'instructions IR simples (`ldconst`, `copy`, `add`, `sub`, etc.).
- **Gestion des variables** : chaque variable est allouée sur la pile. L'offset par rapport à `%rbp` est calculé par le CFG et traduit en `-offset(%rbp)`.
- **Convention d'appel** : prologue/épilogue standard (`push %rbp` / `mov %rsp, %rbp` / `leave` / `ret`).
- **Résultat** : la valeur de retour est placée dans `%eax`.

---

## Structure des fichiers

```
PLD_COMP/
├── compiler/
│   ├── ifcc.g4                  # Grammaire ANTLR4 du langage
│   ├── main.cpp                 # Point d'entrée : parsing, STV puis CGV
│   ├── type.h                   # Enum des types (INT32, CHAR, VOID)
│   ├── IR.h / IR.cpp            # Classes IRInstr, BasicBlock, CFG + gen_asm x86 et MSP430
│   ├── CFG_MSP430.h / .cpp      # Sous-classe CFG pour MSP430
│   ├── SymbolTableVisitor.h/.cpp # Analyse sémantique
│   ├── CodeGenVisitor.h/.cpp    # Construction IR + gestion des CFG
│   ├── Makefile
│   ├── generated/               # Code C++ généré par ANTLR4 (ne pas modifier)
│   └── build/                   # Fichiers de dépendances (.d)
├── testfiles/
│   ├── DEMO/                    # Programmes exemples complets (voir ci-dessous)
│   ├── KnownBugs/               # Bugs identifiés du compilateur (voir ci-dessous)
│   ├── NotImplementedYet/       # Fonctionnalités non supportées (voir ci-dessous)
│   └── TestsOk/                 # Tests par fonctionnalité (voir ci-dessous)
├── ifcc-test.py                 # Script de test automatisé x86-64
├── ifcc-test-msp430.py          # Script de test automatisé MSP430
└── README.md
```

### Arborescence des fichiers de tests

Les tests sont organisés en quatre dossiers à la racine de `testfiles/` :

**`DEMO/`** — programmes exemples complets illustrant les capacités du compilateur. Servent de démonstration de bout en bout.

**`TestsOk/`** — tests unitaires par fonctionnalité, organisés en sous-dossiers thématiques :

```
TestsOk/
├── tests_expressions/           # Opérateurs et expressions
├── tests_return/                # return depuis divers contextes
├── tests_bloc/                  # Portées imbriquées et shadowing
├── tests_conditionnelles/       # if/else et while
│   ├── tests_if_else/
│   └── tests_while/
├── tests_function/              # Fonctions, paramètres, void
├── tests_fonctions_coherence/   # Cohérence sémantique des appels
├── tests_putchar_getchar/       # putchar et getchar
├── tests_switch/                # switch...case
├── tests_break/                 # break
├── tests_continue/              # continue
├── tests_tableau_une_dimension/ # Tableaux 1D
├── tests_operateurs_aff_incr_decr/ # +=, -=, ++, --
├── tests_operateurs_logiques/   # ||, &&
└── tests_declaration_affectation_combine/ # Init, folding, portées
```

Pour chaque test, ifcc et GCC sont compilés et exécutés, le test passe si les sorties correspondent. Pour les programmes invalides (erreurs de compilation attendues), le test passe si les deux rejettent le programme.

**`KnownBugs/`** — bugs identifiés dans le compilateur. Ces programmes sont valides pour GCC mais ifcc produit un résultat incorrect ou différent. Ils documentent les limites connues du compilateur.

**`NotImplementedYet/`** — fonctionnalités non supportées. Ces programmes sont valides pour GCC mais sont rejetés par ifcc car la fonctionnalité n'a pas été implémentée (manque de temps, hors périmètre).

---

### Différences de couverture entre x86-64 et MSP430

Le backend MSP430 a été ajouté en fin de projet et n'a pas pu bénéficier du même niveau de finition que le backend x86-64.

#### Résultats globaux

| Cible | Tests validés | Tests skippés | Échecs |
|---|---|---|---|
| x86-64 | majorité des tests | 0 | `array_oob`, `test_error_missing_arg` |
| MSP430 | 184 / 207 | 45 (mul/div) | 23 |

#### Tests échouant sur les deux architectures

| Test | Raison |
|---|---|
| `testfiles/tests_tableau_une_dimension/array_oob.c` | Comportement hors-limites de tableau non géré |
| `testfiles/tests_function/test_error_missing_arg.c` | Cas-limite de détection d'argument manquant non traité |

#### Tests skippés sur MSP430 uniquement (mul/div absents)

Le MSP430 ne dispose pas d'instructions matérielles de multiplication ou de division. Tout test impliquant `*`, `/` ou `%` sur des variables non réductibles à la compilation est automatiquement ignoré par `ifcc-test-msp430.py`.

#### Tests échouant sur MSP430 mais pas sur x86-64

**1. Limitation du nombre de paramètres de fonctions** — sur MSP430, seuls 3 registres sont disponibles pour le passage de paramètres (R12, R13, R14), contre 6 sur x86-64. Les fonctions nécessitant plus de 3 arguments ne sont pas supportées.

**2. `putchar` non supporté sur MSP430** — `putchar` repose sur un appel système (`write` via la libc) non disponible dans l'environnement de simulation `mspdebug`.

**3. Opérateur logique `||` / `&&`** — un test produit des résultats différents entre MSP430 et x86, révélant une divergence dans la gestion des courts-circuits sur 16 bits ([`testfiles/tests_operateurs_logiques/5.c`](testfiles/tests_operateurs_logiques/5.c)).

---

## Méthodologie

Le projet a été développé en **sprints courts** (approche agile), chacun produisant un compilateur fonctionnel de bout en bout sur un sous-ensemble bien défini du C. Chaque sprint était précédé par l'écriture de tests (approche TDD).

---

*Ce fichier README.md a été rédigé à l'aide d'une IA (Claude Sonnet 4.6).*