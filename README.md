# Rapport du projet compilateur IFCC

## 1. Fonctionnalités implémentées

### Architecture du compilateur

Le compilateur suit un pipeline classique en trois étapes :

```
Source C  →  [ANTLR4 Lexer/Parser]  →  [SymbolTableVisitor]  →  [CodeGenVisitor]  →  Assembleur x86-64
```

1. **Analyse lexicale et syntaxique** : assurée par ANTLR4 à partir de la grammaire [`compiler/ifcc.g4`](compiler/ifcc.g4).
2. **Analyse sémantique** : effectuée par [`SymbolTableVisitor`](compiler/SymbolTableVisitor.cpp) (premier passage sur l'AST).
3. **Génération de code** : effectuée par [`CodeGenVisitor`](compiler/CodeGenVisitor.cpp) (deuxième passage).

---

### Fonctionnalités du langage supportées

#### Types
| Type | Support |
|------|---------|
| `int` (32 bits) | ✅ |

#### Instructions
| Construction | Exemple | Fichier de test |
|---|---|---|
| Entier littéral | `return 42;` | [1_return42.c](testfiles/1_return42.c) |
| Déclaration de variable | `int a;` | [3_return_var.c](testfiles/3_return_var.c) |
| Déclaration multiple | `int a, b, c;` | [21_multi_var_chain.c](testfiles/21_multi_var_chain.c) |
| Affectation | `a = expr;` | [3_return_var.c](testfiles/3_return_var.c) |
| Instruction `return` | `return expr;` | [1_return42.c](testfiles/1_return42.c) |

#### Opérateurs arithmétiques
| Opérateur | Description | Fichier de test |
|---|---|---|
| `+`, `-` | Addition, soustraction | [6_left_assoc_sub.c](testfiles/6_left_assoc_sub.c) |
| `*` | Multiplication | [5_mul_priority.c](testfiles/5_mul_priority.c) |
| `/` | Division entière | [8_div_mod.c](testfiles/8_div_mod.c) |
| `%` | Modulo | [8_div_mod.c](testfiles/8_div_mod.c) |
| `-expr` | Négation unaire | [9_unary.c](testfiles/9_unary.c) |
| `!expr` | NOT logique | [9_unary.c](testfiles/9_unary.c) |

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

### Analyse sémantique (table des symboles)

Le [`SymbolTableVisitor`](compiler/SymbolTableVisitor.cpp) effectue, en un seul passage, les vérifications suivantes :

| Erreur / Avertissement | Message | Fichier de test |
|---|---|---|
| Double déclaration | `Erreur : variable 'x' déjà déclarée.` | [23_invalid_double_decl.c](testfiles/23_invalid_double_decl.c) |
| Variable non déclarée à droite d'une expression | `Erreur : variable 'x' utilisée sans déclaration.` | [24_invalid_undecl_rhs.c](testfiles/24_invalid_undecl_rhs.c) |
| Variable non déclarée à gauche d'une affectation | `Erreur : variable 'x' utilisée sans déclaration.` | [25_invalid_undecl_lhs.c](testfiles/25_invalid_undecl_lhs.c) |
| Variable déclarée mais jamais utilisée | `Avertissement : variable 'x' déclarée mais jamais utilisée.` | — |
| Erreur de syntaxe | `error: syntax error during parsing` | [2_invalid_program.c](testfiles/2_invalid_program.c) |

---

### Génération de code x86-64 (AT&T)

Le [`CodeGenVisitor`](compiler/CodeGenVisitor.cpp) produit de l'assembleur x86-64 en syntaxe AT&T, compatible Linux (et macOS via `#ifdef __APPLE__`).

- **Gestion des variables** : chaque variable `int` est allouée sur la pile par `sub $4, %rsp`. L'offset de chaque variable par rapport à `%rbp` est pré-calculé par `SymbolTableVisitor` puis inversé dans le constructeur de `CodeGenVisitor`.
- **Variables temporaires** : la méthode `allocTemp()` alloue dynamiquement un slot sur la pile pour stocker les résultats intermédiaires lors de l'évaluation des opérations binaires, puis le libère immédiatement après usage (`add $4, %rsp`).
- **Convention d'appel** : prologue/épilogue standard (`push %rbp` / `mov %rsp, %rbp` / `pop %rbp`).
- **Résultat** : toute expression laisse son résultat dans `%eax`.

---

## 2. Navigation dans le code

### Structure des fichiers

```
c:\PLD-COMP\
├── compiler/
│   ├── ifcc.g4                  # Grammaire ANTLR4 du langage
│   ├── main.cpp                 # Point d'entrée : parsing, STV puis CGV
│   ├── SymbolTableVisitor.h     # Interface de l'analyse sémantique
│   ├── SymbolTableVisitor.cpp   # Vérification des déclarations / usages
│   ├── CodeGenVisitor.h         # Interface de la génération de code
│   ├── CodeGenVisitor.cpp       # Génération assembleur x86-64
│   ├── Makefile                 # Build du compilateur
│   ├── generated/               # Code C++ généré par ANTLR4 (ne pas modifier)
│   │   ├── ifccLexer.{h,cpp}
│   │   ├── ifccParser.{h,cpp}
│   │   ├── ifccBaseVisitor.{h,cpp}
│   │   └── ifccVisitor.{h,cpp}
│   └── build/                   # Fichiers de dépendances (.d)
├── testfiles/                   # Programmes de test C
│   ├── 1_return42.c             # Retour d'une constante
│   ├── 2_invalid_program.c      # Syntaxe invalide
│   ├── 3_return_var.c           # Variable et retour
│   ├── 4_invalid_undecl_var.c   # Variable non déclarée
│   ├── 5_mul_priority.c         # Priorité * sur +
│   ├── 6_left_assoc_sub.c       # Associativité gauche de -
│   ├── 7_parens_priority.c      # Parenthèses
│   ├── 8_div_mod.c              # Division et modulo
│   ├── 9_unary.c                # Opérateurs unaires
│   ├── 10_bitwise.c             # Opérateurs bit-à-bit
│   ├── 11_comparisons.c         # < et >
│   ├── 12_char_const.c          # Constante char ('Z' - 'A')
│   ├── 13_char_escape.c         # Char escape ('\n')
│   ├── 14_equality.c            # == et !=
│   ├── 15_combined_expr.c       # Expression complexe combinée
│   ├── 16_left_assoc_div.c      # Associativité gauche de /
│   ├── 17_unary_minus_var.c     # Négation d'une variable
│   ├── 18_unary_on_expr.c       # Unaire sur expression
│   ├── 19_nested_parens.c       # Parenthèses imbriquées
│   ├── 20_char_plus_int.c       # Char + int
│   ├── 21_multi_var_chain.c     # Déclarations multiples
│   ├── 22_cmp_in_arith.c        # Comparaison dans une expression
│   ├── 23_invalid_double_decl.c # Erreur : double déclaration
│   ├── 24_invalid_undecl_rhs.c  # Erreur : non déclarée en lecture
│   └── 25_invalid_undecl_lhs.c  # Erreur : non déclarée en écriture
└── ifcc-test.py                 # Script de test automatisé
```

### Points d'entrée clés dans le code

| Fonctionnalité | Fichier | Emplacement |
|---|---|---|
| Grammaire complète | [compiler/ifcc.g4](compiler/ifcc.g4) | L.1–32 |
| Main / pipeline complet | [compiler/main.cpp](compiler/main.cpp) | L.1–62 |
| Construction de la table des symboles | [compiler/SymbolTableVisitor.cpp](compiler/SymbolTableVisitor.cpp) | `visitDeclar` L.3 |
| Vérification des usages | [compiler/SymbolTableVisitor.cpp](compiler/SymbolTableVisitor.cpp) | `visitExprId` L.33 |
| Avertissement variables inutilisées | [compiler/SymbolTableVisitor.cpp](compiler/SymbolTableVisitor.cpp) | `visitProg` L.46 |
| Prologue/épilogue assembleur | [compiler/CodeGenVisitor.cpp](compiler/CodeGenVisitor.cpp) | `visitProg` L.33 |
| Allocation de temporaires | [compiler/CodeGenVisitor.cpp](compiler/CodeGenVisitor.cpp) | `allocTemp` L.26 |
| Opérations binaires (+, -) | [compiler/CodeGenVisitor.cpp](compiler/CodeGenVisitor.cpp) | `visitExprAdd` L.114 |
| Multiplication, division, modulo | [compiler/CodeGenVisitor.cpp](compiler/CodeGenVisitor.cpp) | `visitExprMult` L.134 |
| Opérateurs bit-à-bit | [compiler/CodeGenVisitor.cpp](compiler/CodeGenVisitor.cpp) | `visitExprBitOr/Xor/And` L.157 |
| Comparaisons (`<`, `>`, `==`, `!=`) | [compiler/CodeGenVisitor.cpp](compiler/CodeGenVisitor.cpp) | `visitExprCmp/Eq` L.207 |

---

## 3. Gestion de projet

### Travail réalisé

| Étape | Contenu |
|---|---|
| Mise en place | Initialisation du projet avec ANTLR4, Makefile, grammaire minimale (`return` entier). |
| Expressions | Ajout de toutes les expressions arithmétiques, unaires, bit-à-bit et de comparaison avec gestion de la priorité dans la grammaire. |
| Variables | Support des déclarations (`int`) simples et multiples, affectations, lecture de variables dans les expressions. |
| Table des symboles | Implémentation du `SymbolTableVisitor` : double déclaration, variable non déclarée, variable inutilisée. |
| Génération de code | `CodeGenVisitor` complet : allocation de variables sur la pile, temporaires dynamiques, assembleur x86-64 AT&T. |
| Tests | Rédaction de 25 fichiers de test couvrant les cas valides et invalides. |

### Travail à venir

| Priorité | Fonctionnalité |
|---|---|
| Haute | Support des constantes caractères (`'A'`, `'\n'`) dans la grammaire (tests 12, 13, 20 actuellement non couverts par le lexer). |
| Haute | Représentation intermédiaire (IR / 3-adresses) pour découpler l'analyse du backend et préparer les optimisations. |
| Moyenne | Structures de contrôle : `if`/`else`, `while`. |
| Moyenne | Type `char` (store/load 8 bits, cast implicite vers `int`). |
| Moyenne | Appels de fonctions (au moins `putchar`/`getchar` pour les entrées/sorties). |
| Basse | Optimisations : propagation de constantes, élimination de code mort. |
| Basse | Meilleure gestion des erreurs : numéros de ligne dans les messages d'erreur sémantique. |
