# Plan de route — Passage à l'IR (section 4.8)

## Vue d'ensemble de l'architecture

```
AST (front-end)  →  IR (CFG + BasicBlocks + IRInstr)  →  x86 / ARM / MSP430
```

---

## Les 3 grandes tâches (certaines parallélisables)

### Tâche A — Mise en place de la structure IR
*(1 personne, bloquante pour tout le reste)*

C'est le socle. Il faut implémenter les classes du fichier `IR.h` :

- **`IRInstr`** : constructeur + `gen_asm()` pour chaque opération (`ldconst`, `copy`, `add`, `sub`, `mul`, `rmem`, `wmem`, `call`, `cmp_eq`, `cmp_lt`, `cmp_le`)
- **`BasicBlock`** : constructeur + `add_IRInstr()` + `gen_asm()` avec la logique de saut (`exit_true` / `exit_false`)
- **`CFG`** : constructeur + `add_bb()` + gestion de la table des symboles (`add_to_symbol_table`, `create_new_tempvar`, `get_var_index`, etc.)

> ⚠️ C'est le **point de départ critique** — rien d'autre ne peut avancer sans ça.

---

### Une fois Tâche A terminée → lancer en parallèle :

```
        ┌──── Tâche B (front-end → IR)
Tâche A ─┤
        └──── Tâche C (IR → x86)  ──→  Tâche D (IR → ARM/MSP430)
```

---

### Tâche B — Front-end : faire construire l'IR par les visiteurs AST
*(1-2 personnes)*

Modifier les visiteurs existants pour qu'au lieu d'émettre de l'assembleur directement, ils appellent `cfg->current_bb->add_IRInstr(...)`.

Ordre suggéré :
1. Expressions simples : constantes (`ldconst`), variables (`copy`)
2. Opérations arithmétiques : `add`, `sub`, `mul`
3. Affectations : `wmem` / `rmem`
4. Appels de fonctions : `call`
5. Structures de contrôle : `if`, `while` → gestion de `exit_true` / `exit_false` + `test_var_name`

---

### Tâche C — Back-end x86 : IR → assembleur x86
*(1 personne, en parallèle avec B)*

Implémenter `IRInstr::gen_asm()` pour la cible x86, et `CFG::gen_asm_prologue/epilogue()`.

Pour chaque opération IR, écrire la traduction x86 correspondante. Exemples :
- `ldconst d, c` → `movl $c, d(%rbp)`
- `add d, x, y` → charger x, y, addl, stocker dans d
- `cmp_eq` → `cmpl` + `sete` + `movzbl`
- Sauts : générés par `BasicBlock::gen_asm()` selon `exit_true`/`exit_false`

---

### Tâche D — Back-end alternatif : IR → ARM ou MSP430
*(1-2 personnes, commence après Tâche A, nécessite une machine/simulateur)*

Même logique que Tâche C mais pour une autre architecture. Choisir selon disponibilité matérielle :

| Cible | Prérequis | Complexité |
|---|---|---|
| ARM (Mac Apple Silicon) | Machine dispo | ⭐ simple, natif |
| ARM (Raspberry Pi / Android) | Matériel dispo | ⭐⭐ |
| MSP430 | `gcc-msp430` + simulateur `mspsim` | ⭐⭐⭐ |
| Bytecode Java | JDK installé | ⭐⭐ |

> Pour MSP430 : `int` peut être 16 ou 32 bits, et la **multiplication peut être refusée** proprement si non supportée.

---

## Résumé du planning

```
Semaine 1 :  [A] Structure IR  (1 personne, tout seul)
Semaine 2+ : [B] Front-end AST→IR   (en parallèle)
             [C] Back-end IR→x86    (en parallèle)
             [D] Back-end IR→ARM    (en parallèle, si matériel prêt)
```

> **Note importante** : pour l'instant, rester dans un seul BasicBlock (code linéaire, sans `if`/`while`). Le graphe de flot de contrôle (plusieurs BB reliés) vient dans une tâche ultérieure.