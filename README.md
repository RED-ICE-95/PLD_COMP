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

---

### Limites connues du compilateur (x86-64)

Le compilateur implémente un **sous-ensemble significatif du langage C**, mais certaines constructions standard ne sont pas supportées par manque de temps. Ces limitations sont identifiées, documentées et accompagnées de tests dans le dossier [`testfiles/tests_failed/`](testfiles/tests_failed/). Ces programmes sont tous compilables par GCC mais sont rejetés ou mal compilés par ifcc.

#### Boucles

| Limitation | Fichier de test |
|---|---|
| Boucle `for` non supportée (seul `while` est implémenté) | [01_for_loop.c](testfiles/tests_failed/01_for_loop.c) |
| Boucle `do-while` non supportée | [02_do_while.c](testfiles/tests_failed/02_do_while.c) |
| Boucles `for` imbriquées | [20_for_nested.c](testfiles/tests_failed/20_for_nested.c) |

#### Opérateurs manquants

| Limitation | Fichier de test |
|---|---|
| Opérateur ternaire `? :` | [03_ternary_operator.c](testfiles/tests_failed/03_ternary_operator.c) |
| Opérateurs de décalage `<<` et `>>` | [11_bitshift.c](testfiles/tests_failed/11_bitshift.c) |
| Opérateur virgule `(a, b)` | [16_comma_operator.c](testfiles/tests_failed/16_comma_operator.c) |
| Comparaisons `<=` et `>=` (seuls `<` et `>` sont dans la grammaire) | [19_cmp_le_ge.c](testfiles/tests_failed/19_cmp_le_ge.c), [30_cmp_ge.c](testfiles/tests_failed/30_cmp_ge.c) |
| `sizeof` | [10_sizeof.c](testfiles/tests_failed/10_sizeof.c) |
| Pré-incrémentation dans une expression (`int b = ++a`) | [27_pre_increment_expr.c](testfiles/tests_failed/27_pre_increment_expr.c) |
| Post-incrémentation dans une expression (`int b = a++`) | [28_post_increment_expr.c](testfiles/tests_failed/28_post_increment_expr.c) |

#### Types non supportés

| Limitation | Fichier de test |
|---|---|
| `float` | [08_float.c](testfiles/tests_failed/08_float.c) |
| `double` | [09_double.c](testfiles/tests_failed/09_double.c) |
| `unsigned int` | [17_unsigned.c](testfiles/tests_failed/17_unsigned.c) |
| `long` | [18_long.c](testfiles/tests_failed/18_long.c) |
| Paramètre de type `char` (seul `int` est accepté comme type de paramètre) | [29_char_param.c](testfiles/tests_failed/29_char_param.c) |

#### Pointeurs et mémoire

| Limitation | Fichier de test |
|---|---|
| Déclaration de pointeurs (`int *p = &x`) | [05_pointer_basic.c](testfiles/tests_failed/05_pointer_basic.c) |
| Arithmétique de pointeurs (`p + 2`, `*p`) | [21_pointer_arithmetic.c](testfiles/tests_failed/21_pointer_arithmetic.c) |
| Variables globales | [04_global_variable.c](testfiles/tests_failed/04_global_variable.c) |
| Tableaux multi-dimensionnels (`int arr[2][3]`) | [14_array_2d.c](testfiles/tests_failed/14_array_2d.c) |
| Chaînes de caractères (`"hello"`) | [13_string_literal.c](testfiles/tests_failed/13_string_literal.c) |

#### Constructions du langage non supportées

| Limitation | Fichier de test |
|---|---|
| `struct` | [06_struct.c](testfiles/tests_failed/06_struct.c) |
| `enum` | [07_enum.c](testfiles/tests_failed/07_enum.c) |
| `union` | [25_union.c](testfiles/tests_failed/25_union.c) |
| `typedef` | [15_typedef.c](testfiles/tests_failed/15_typedef.c) |
| Cast explicite (`(int)x`) | [12_cast.c](testfiles/tests_failed/12_cast.c) |
| `goto` | [23_goto.c](testfiles/tests_failed/23_goto.c) |
| Qualificateur `const` | [24_const_qualifier.c](testfiles/tests_failed/24_const_qualifier.c) |
| Variables `static` | [26_static_variable.c](testfiles/tests_failed/26_static_variable.c) |
| Préprocesseur (`#define`) | [22_define.c](testfiles/tests_failed/22_define.c) |

> **Note** : Ces limitations sont inhérentes au périmètre choisi pour le projet (sous-ensemble du C) et non des bugs. Elles auraient pu être implémentées avec davantage de temps.

#### Cas limites dans les fonctionnalités implémentées

En plus des constructions non implémentées, certains **cas limites** au sein des fonctionnalités existantes produisent des résultats incorrects ou des erreurs de parsing inattendues. Tous les tests ci-dessous **échouent** effectivement avec ifcc.

**Résultat d'exécution incorrect**

| Cas limite | Description | Fichier de test |
|---|---|---|
| `return` implicite absent | `int main() { int x = 42; }` — pas de `return` → valeur indéterminée (C99 impose `return 0` implicite dans `main`) | [31_missing_return.c](testfiles/tests_failed/31_missing_return.c) |
| Opérateur `~` (NOT bit-à-bit) | `~x` est accepté par le parser (ANTLR error recovery) mais produit un résultat incorrect à l'exécution | [39_bitwise_not.c](testfiles/tests_failed/39_bitwise_not.c) |

**Limitations de la grammaire sur des constructions supportées**

| Cas limite | Description | Fichier de test |
|---|---|---|
| `<=` et `>=` absents | La grammaire n'inclut que `<` et `>` ; `<=` provoque une erreur de parsing | [19_cmp_le_ge.c](testfiles/tests_failed/19_cmp_le_ge.c), [30_cmp_ge.c](testfiles/tests_failed/30_cmp_ge.c) |
| `while (i <= n)` impossible | Boucle classique inutilisable à cause de l'absence de `<=` | [35_le_ge_in_loop.c](testfiles/tests_failed/35_le_ge_in_loop.c) |
| `return;` dans fonction `void` | La grammaire exige `return expr;` — un `return;` seul est rejeté | [32_void_return_no_expr.c](testfiles/tests_failed/32_void_return_no_expr.c) |
| Déclaration de variable `char` | Seul `int` est accepté comme type de déclaration ; `char c = 'A';` est rejeté | [33_char_variable_decl.c](testfiles/tests_failed/33_char_variable_decl.c) |
| `case` négatif dans `switch` | Les `case` n'acceptent que des constantes `[0-9]+` ; `case -1:` est rejeté | [40_switch_negative_case.c](testfiles/tests_failed/40_switch_negative_case.c) |
| `case` avec expression | `case 2+3:` rejeté (seules les constantes littérales sont acceptées) | [43_switch_const_expr.c](testfiles/tests_failed/43_switch_const_expr.c) |
| Affectation dans une condition | `if (x = 5)` rejeté car l'affectation n'est pas une expression | [38_assign_in_condition.c](testfiles/tests_failed/38_assign_in_condition.c) |
| Affectation chaînée | `a = b = c = 5;` rejeté car l'affectation est un statement, pas une expression | [37_chained_assign.c](testfiles/tests_failed/37_chained_assign.c) |
| Affectation composée dans une expression | `int b = (a += 3);` rejeté | [34_compound_assign_in_expr.c](testfiles/tests_failed/34_compound_assign_in_expr.c) |

**Fonctions : ordre de définition**

| Cas limite | Description | Fichier de test |
|---|---|---|
| Prototypes (forward declarations) | Non supportés ; impossible de déclarer une fonction avant sa définition | [41_forward_declaration.c](testfiles/tests_failed/41_forward_declaration.c) |
| Ordre de définition imposé | Si `f()` appelle `g()` définie plus bas, erreur « fonction non déclarée » | [42_function_order.c](testfiles/tests_failed/42_function_order.c) |
| Récursion mutuelle | Impossible sans forward declaration (`est_pair` ↔ `est_impair`) | [36_mutual_recursion.c](testfiles/tests_failed/36_mutual_recursion.c) |

---

### Différences de couverture entre x86-64 et MSP430

Le backend MSP430 a été ajouté en fin de projet et n'a pas pu bénéficier du même niveau de finition que le backend x86-64. Cette section documente les écarts entre les deux cibles.

#### Résultats globaux

| Cible | Tests validés | Tests skippés | Échecs |
|---|---|---|---|
| x86-64 | majorité des tests | 0 | `array_oob`, `test_error_missing_arg` |
| MSP430 | 184 / 207 | 45 (mul/div) | 23 |

#### Tests échouant sur les deux architectures

Ces deux tests reflètent des comportements non encore implémentés indépendamment de la cible :

| Test | Raison |
|---|---|
| `testfiles/tests_tableau_une_dimension/array_oob.c` | Comportement hors-limites de tableau non défini / non géré |
| `testfiles/tests_function/test_error_missing_arg.c` | Cas-limite de détection d'argument manquant non traité |

#### Tests skippés sur MSP430 uniquement (mul/div absents)

Le MSP430 ne dispose pas d'instructions matérielles de multiplication ou de division. Tout test impliquant `*`, `/` ou `%` sur des variables (non réductibles à la compilation) est donc automatiquement ignoré par `ifcc-test-msp430.py`. Les 45 tests skippés appartiennent principalement aux catégories suivantes :

- `tests_expressions/` : tests mêlant multiplication/division à d'autres opérateurs
- `tests_declaration_affectation_combine/` : initialisations avec `*` ou `/`
- `tests_conditionnelles/` : conditions contenant des multiplications
- `tests_operateurs_aff_incr_decr/` et `tests_operateurs_logiques/` : expressions avec `*`/`/`
- `DEMO/` : plusieurs programmes de démonstration utilisant ces opérations

#### Tests échouant sur MSP430 mais pas sur x86-64

**1. Limitation du nombre de paramètres de fonctions**

Sur x86-64, jusqu'à 6 arguments sont passés dans des registres (`%rdi`, `%rsi`, `%rdx`, `%rcx`, `%r8`, `%r9`). Sur MSP430, seuls 3 registres sont utilisés pour le passage des paramètres (`R12`, `R13`, `R14`). Les fonctions nécessitant plus de 3 arguments ne sont pas supportées sur MSP430, ce qui provoque soit un rejet erroné de programmes valides, soit une génération d'assembleur incorrecte :

| Test | Échec |
|---|---|
| `testfiles/tests_fonctions_coherence/06_plus_de_6_args.c` | Rejette un programme valide |
| `testfiles/tests_fonctions_coherence/07_ok_6_args.c` | Rejette un programme valide |
| `testfiles/tests_fonctions_coherence/11_nbr_pair.c` | Rejette un programme valide |
| `testfiles/tests_fonctions_coherence/12_nbr_impair.c` | Rejette un programme valide |
| `testfiles/tests_fonctions_coherence/13_6_registres.c` | Rejette un programme valide |
| `testfiles/tests_fonctions_coherence/14.c` | Rejette un programme valide |
| `testfiles/tests_fonctions_coherence/15_fcts_imbriquees.c` | Rejette un programme valide |
| `testfiles/tests_fonctions_coherence/16_negative_value.c` | Rejette un programme valide |
| `testfiles/tests_function/79_too_much_param.c` | Rejette un programme valide |

**2. `putchar` non supporté sur MSP430**

`putchar` repose sur un appel système (`write` via la libc) qui n'est pas disponible dans l'environnement de simulation MSP430 (`mspdebug`). L'appel produit un assembleur que le simulateur ne peut pas exécuter correctement :

| Test | Échec |
|---|---|
| `testfiles/tests_putchar_getchar/27_putchar_simple.c` | Assembleur incorrect |
| `testfiles/tests_putchar_getchar/28_putchar_char.c` | Assembleur incorrect |
| `testfiles/tests_putchar_getchar/29_putchar_expr.c` | Assembleur incorrect |
| `testfiles/tests_putchar_getchar/30_putchar_multiple.c` | Assembleur incorrect |
| `testfiles/tests_putchar_getchar/32_getchar_putchar.c` | Assembleur incorrect |
| `testfiles/tests_putchar_getchar/33_char_arithmetic.c` | Assembleur incorrect |
| `testfiles/tests_putchar_getchar/34_putchar_newline.c` | Assembleur incorrect |

**3. Fonctions avec structure complexe ou passage d'arguments > 3**

Certains tests du dossier `DEMO/` et de `tests_function/` mettent en jeu des fonctions avec plusieurs arguments ou des schémas d'appel imbriqués qui ne sont pas correctement gérés par le backend MSP430 :

| Test | Échec |
|---|---|
| `testfiles/DEMO/Autres_Tests/1.c` | Assembleur incorrect |
| `testfiles/DEMO/Autres_Tests/test_fibo.c` | Assembleur incorrect |
| `testfiles/DEMO/Tests_Sujet/test_1_sujet.c` | Assembleur incorrect |
| `testfiles/tests_function/test_function_args.c` | Assembleur incorrect |

**4. Opérateur logique `||` / `&&`**

Un test d'opérateur logique produit des résultats d'exécution différents entre MSP430 et x86, révélant une divergence dans la gestion des courts-circuits ou de la représentation booléenne sur 16 bits :

| Test | Échec |
|---|---|
| `testfiles/tests_operateurs_logiques/5.c` | Résultats différents à l'exécution |

---

### Source
Ce fichier README.md a été rédigé à l'aide d'une IA (Claude Claude 4.6).
