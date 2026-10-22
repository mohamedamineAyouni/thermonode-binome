# MC/DC — décision de défaut EF-05

Décision testée : `D = A || B || C`

- `A` : température inférieure à −40,00 °C
- `B` : température supérieure à 125,00 °C
- `C` : au moins 5 mesures identiques consécutives
- `D` : défaut détecté

| Test | A | B | C | D | Condition dont l’effet est vérifié |
|---|---|---|---|---|---|
| T1 | Faux | Faux | Faux | Faux | Référence |
| T2 | Vrai | Faux | Faux | Vrai | A, comparé à T1 |
| T3 | Faux | Vrai | Faux | Vrai | B, comparé à T1 |
| T4 | Faux | Faux | Vrai | Vrai | C, comparé à T1 |

Pour enregistrer : **Ctrl+O**, **Entrée**, puis **Ctrl+X**. Ensuite, reviens au terminal et dis-moi « c’est fait ».
