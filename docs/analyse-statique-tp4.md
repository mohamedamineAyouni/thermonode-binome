# Analyse statique — TP4

## Environnement et résultats initiaux

Environnement : VM Ubuntu 24.04, cppcheck installé dans la VM.

- cppcheck classique : 0 constat.
- Addon MISRA C:2012 : 64 constats initiaux répartis sur 16 règles.
- Tests firmware après corrections : 76 réussites, 0 échec.

## Constats et traitements

| Règle | Catégorie | Constats initiaux | Traitement |
|---|---|---:|---|
| 15.5 | Advisory | 25 | Retours anticipés conservés pour traiter les erreurs et protéger les accès ; dérogation DEV-004 documentée et suppression ciblée. |
| 17.7 | Required | 9 | Valeurs de retour vérifiées ou ignorées explicitement ; corrigé dans le code. |
| 5.9 | Advisory | 4 | Identifiants de portée fichier renommés pour éviter les conflits ; corrigé dans le code. |
| 18.4 | Advisory | 4 | Arithmétique de pointeurs du parseur encadrée par les contrôles de longueur ; dérogation DEV-005 documentée et suppression ciblée. |
| 10.8 | Required | 4 | Conversions d’expressions composées corrigées. |
| 12.3 | Advisory | 3 | Déclarations séparées et opérateurs virgule retirés. |
| 12.1 | Advisory | 3 | Parenthèses ajoutées pour clarifier la précédence. |
| 8.9 | Advisory | 2 | Objets déplacés dans les fonctions qui les utilisent. |
| 8.7 | Advisory | 2 | Faux positifs liés aux fonctions appelées par les tests et la HAL, hors périmètre analysé ; suppression ciblée documentée. |
| 21.6 | Required | 2 | Usage de \`snprintf\` documenté par DEV-002 et suppressions ciblées. |
| 7.4 | Required | 1 | Faux positif sur le tableau \`node_id\` ; suppression locale commentée. |
| 22.8 | Required | 1 | \`errno\` remis à zéro avant les conversions numériques. |
| 21.10 | Required | 1 | Usage POSIX réservé au simulateur hôte ; dérogation DEV-006 documentée. |
| 21.1 | Required | 1 | Macro réservée utilisée par le simulateur hôte ; dérogation DEV-006 documentée. |
| 2.5 | Advisory | 1 | Limite de période remplacée par la macro prévue ; constat restant sur la macro POSIX du simulateur hôte traité par DEV-006. |
| 13.3 | Advisory | 1 | Affectation de \`f.seq\` séparée de l’incrémentation de \`seq\`. |

La règle 22.9 a également été signalée pendant les corrections de conversion numérique. Le code vérifie maintenant \`errno\` après la conversion.

## Vérification finale

Après corrections et suppressions ciblées, l’analyse MISRA ne signale plus de constat parmi les règles traitées. Les suppressions sont listées dans \`firmware/misra-suppressions.txt\` et leurs justifications sont dans \`docs/misra-deviations.md\`.

La suite firmware a réussi : 76 tests, 0 échec. Les suppressions MISRA ne remplacent pas les tests fonctionnels ; elles documentent les dérogations et faux positifs retenus.
