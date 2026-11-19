# Revue de MR !7 « fix hysteresis » — réviseurs : Ayouni Mohamedamine

Branche examinée : `fix/EF-04-hysteresis`
Commit examiné : `6db598e`

| # | Fichier:ligne | Constat | Règle / source | Gravité | Demande à l’auteur |
|---|---|---|---|---|---|
| 1 | `firmware/src/node.c:22` | La copie avec une taille de `TN_ID_LEN` ne garantit pas le caractère nul si l’identifiant remplit tout le tampon. Le changement est aussi sans rapport avec l’hystérésis. | Revue des chaînes et tampons ; périmètre de la MR | Majeure | Garder la terminaison explicite et retirer ce changement hors sujet. |
| 2 | `firmware/src/threshold.c:31` | La constante `500` ignore `hyst_centi` ; l’hystérésis configurée n’est plus utilisée. | EF-04 ; liste de contrôle « constantes » | Bloquante | Utiliser la valeur d’hystérésis configurée et garder le test actif. |
| 3 | `firmware/src/threshold.c:32` | L’opérateur virgule combine deux actions dans une expression. | MISRA C:2012, règle 12.3 | Majeure | Séparer les instructions dans un bloc explicite. |
| 4 | `firmware/src/threshold.c:31–38` | Les corps des conditions `if` et `else` ne sont pas encadrés par des accolades. | MISRA C:2012, règle 15.6 | Majeure | Remettre les accolades autour des corps concernés. |
| 5 | `firmware/test/test_threshold.c:55,87` | Le test de l’hystérésis configurable est renommé `xtest...` et son appel est commenté. La suite ne vérifie donc plus ce comportement. | EF-04 ; tests de régression | Bloquante | Réactiver le test et corriger le code jusqu’à ce qu’il passe. |

## Rapports de défaut

### DR-TP4-01 — Hystérésis configurable ignorée

- **Commit :** `6db598e`
- **Environnement :** VM Ubuntu 24.04, GCC 13
- **Référence :** EF-04
- **Étapes :** configurer `threshold_set(6000, 300)`, puis appeler `threshold_step(5699, true)`.
- **Résultat attendu :** `THR_CLEAR` (`2`), car 5699 est inférieur à 6000 − 300.
- **Résultat obtenu :** `THR_NONE` (`0`), car le patch compare à 6000 − 500.
- **Preuve :** après réactivation temporaire du test, `make -C firmware test` échoue : `Expected 2 Was 0`. Le test a ensuite été remis à l’état désactivé de la MR.
- **Impact / gravité :** l’alerte reste active alors que la température a franchi la limite d’hystérésis configurée ; comportement fonctionnel incorrect.
- **Priorité :** bloquante avant fusion.

### DR-TP4-02 — Identifiant potentiellement non terminé

- **Commit :** `6db598e`
- **Environnement :** VM Ubuntu 24.04, GCC 13
- **Référence :** revue des chaînes et tampons
- **Étapes :** appeler `node_set_id` avec un identifiant d’au moins `TN_ID_LEN` caractères, puis utiliser cet identifiant pour construire une trame.
- **Résultat attendu :** l’identifiant est limité à la taille du tampon et terminé par `\0`.
- **Résultat obtenu :** avec `strncpy(node_id, id, TN_ID_LEN)`, un identifiant qui remplit le tampon peut ne pas être terminé par `\0`. Le risque doit être vérifié par un test aux limites sur la construction de trame.
- **Impact / gravité :** risque de lecture hors limites ou de trame incorrecte lors d’un usage ultérieur comme chaîne.
- **Priorité :** élevée ; retirer ce changement hors sujet et préserver la terminaison.

## Décision

**Demander des changements.** La MR remplace l’hystérésis configurable par une constante et désactive le test qui aurait révélé la régression. Elle ajoute aussi un risque sur la terminaison de l’identifiant et des écarts MISRA. La branche ne doit pas être fusionnée avant correction et validation.

## Q1 — Complémentarité des vérifications

1. `node.c:22` : la revue du diff repère le changement hors périmètre ; un test avec un identifiant de longueur maximale vérifie le risque de terminaison. Les tests existants ne couvrent pas ce cas.
2. `threshold.c:31` : la comparaison à EF-04 et le test ad hoc avec une hystérésis de 300 révèlent la constante codée en dur. La suite normale l’a manqué parce que le test correspondant était désactivé.
3. `threshold.c:32` : l’analyse MISRA, règle 12.3, peut relever l’opérateur virgule. Les tests fonctionnels ne détectent pas ce défaut de style.
4. `threshold.c:31–38` : l’analyse MISRA, règle 15.6, peut relever l’absence d’accolades. Les tests fonctionnels ne détectent pas ce défaut de conformité.
5. `test_threshold.c:55,87` : la revue du diff et le décompte des tests révèlent le test désactivé. La CI exécute les tests enregistrés ; elle ne peut donc pas échouer sur le test retiré.

La revue, les tests dynamiques et l’analyse statique se complètent : les tests vérifient le comportement sur les cas exécutés, les outils statiques vérifient des règles de code, et la revue examine l’intention, le périmètre et les cas absents. Aucune méthode seule ne suffit.
