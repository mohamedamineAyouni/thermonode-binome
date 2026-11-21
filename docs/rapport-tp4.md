# Rapport TP4 — ThermoNode

## Résumé

Ce TP a porté sur la revue de code, l’analyse statique, les tests et la gestion des changements du projet ThermoNode. Les corrections MISRA C:2012, les dérogations conservées et les résultats d’analyse sont détaillés dans `docs/analyse-statique-tp4.md` et `docs/misra-deviations.md`.

Les vérifications finales effectuées dans la VM Ubuntu 24.04 sont : firmware, 76 tests réussis ; gateway, 22 tests réussis ; Pylint, 10/10 ; cppcheck classique, 0 constat. Aucun échec n’a été observé.

## Q1 — Revue de la MR !7

La revue de la MR !7 « fix hysteresis » a relevé cinq problèmes, consignés dans `docs/review-MR7.md`. La décision était de demander des changements avant fusion.

1. Dans `node.c`, une copie de taille `TN_ID_LEN` pouvait ne pas terminer l’identifiant par `\0`. Le changement était également hors du périmètre de l’hystérésis.
2. Dans `threshold.c`, une constante de 500 remplaçait la valeur d’hystérésis configurée et entraînait une régression fonctionnelle.
3. L’opérateur virgule combinait deux actions, contrairement à MISRA C:2012, règle 12.3.
4. Des blocs `if` et `else` ne comportaient pas d’accolades, en écart à la règle 15.6.
5. Un test d’hystérésis configurable avait été désactivé ; la suite ne pouvait donc plus détecter la régression.

Les méthodes se complètent : les tests vérifient les cas exécutés, l’analyse statique détecte certains écarts aux règles, et la revue examine le périmètre, l’intention et les tests manquants. La revue était réalisée par l’auteur lui-même, car le travail a été effectué seul ; elle ne constitue donc pas une revue indépendante.

## Q2 — Analyse statique

L’analyse initiale avec cppcheck classique n’a signalé aucun constat. L’addon MISRA C:2012 a relevé 64 constats répartis sur 16 règles. Les corrections et dérogations sont récapitulées dans `docs/analyse-statique-tp4.md`.

Les écarts pouvant être corrigés ont été traités dans le code : valeurs de retour vérifiées, conversions et précédence clarifiées, opérateur virgule retiré, objets à portée fichier localisés, et gestion d’`errno` corrigée. Les écarts conservés sont justifiés dans `docs/misra-deviations.md` et les suppressions ciblées figurent dans `firmware/misra-suppressions.txt`. Ils concernent notamment les retours anticipés utiles aux contrôles d’erreur, l’analyse de buffers déjà bornés et certains éléments réservés au simulateur hôte ou à la HAL.

Après corrections, l’analyse MISRA ne signale plus de constat parmi les règles traitées. Pylint affiche 10/10 lorsqu’il est lancé depuis `gateway`, où se trouve sa configuration. Le score initial de Pylint n’a pas été conservé dans les éléments disponibles ; aucune comparaison chiffrée avant/après n’est donc avancée ici.

## Q3 — SonarQube et couverture

Le Quality Gate personnalisé `TVD` a été associé au projet ThermoNode. Le rapport d’analyse consigné indique un Quality Gate réussi, les notes A en sécurité, fiabilité et maintenabilité, une couverture de 62,9 % et 0,0 % de duplication.

La couverture importée concerne le gateway Python. Dans cette configuration Community, les fichiers firmware C ne sont pas analysés nativement par SonarQube ; leur contrôle est effectué séparément avec les outils firmware, notamment cppcheck et les tests du Makefile. Un Quality Gate n’a pas nécessairement à intégrer seul chaque vérification : l’ensemble du processus doit couvrir les composants, ce que complètent ici les étapes firmware et gateway. Les résultats Sonar détaillés et la limite de périmètre sont consignés dans `docs/analyse-statique-tp4.md`.

## Q4 — Conflit, poussée forcée et tests

Le changement de l’hystérésis par défaut a été développé sur deux branches. Après le rebase de la branche de documentation sur la branche de correction, Git a signalé un conflit dans `firmware/src/threshold.h`. Il a été résolu en conservant la valeur 400, correspondant à 4 °C, et en adaptant le commentaire EF-04. La suite firmware a ensuite réussi : 76 tests, 0 échec.

La poussée a utilisé `--force-with-lease` après le rebase, car celui-ci réécrit l’historique de la branche. Cette option vérifie que la branche distante n’a pas avancé de façon inattendue ; `--force` écraserait sans cette vérification les changements distants. Les tests existants restent verts parce qu’ils vérifient principalement l’hystérésis configurée explicitement et ne vérifient pas directement la nouvelle valeur par défaut. Un test dédié à la valeur par défaut reste donc nécessaire.

## Q5 — Versionnement et gestion de configuration

Le tag prévu est `v0.2.0`, pour identifier l’état du projet après les changements du TP. Une correction isolée sans changement d’interface correspondrait à une augmentation de correctif, telle que `0.1.1`. `1.0.0` marquerait une première version stable avec un engagement de compatibilité. Une modification incompatible du format de trame ICD doit être annoncée comme une évolution incompatible de l’interface et recevoir une version adaptée à cette rupture.

Deux pull requests ont été fusionnées dans `main` : la PR #1 regroupe les corrections MISRA, la documentation et Pylint ; la PR #2 apporte le changement d’hystérésis et sa résolution de conflit. Le TP demandait plusieurs MR séparées et des revues par un tiers. Comme le travail a été fait seul, ces revues indépendantes n’ont pas pu être réalisées, et les changements ont été regroupés dans deux PR. Cette limite est signalée plutôt que présentée comme une revue par un pair.

## Plan de réduction de dette

1. Ajouter un test vérifiant directement la valeur d’hystérésis par défaut et conserver les tests de la valeur configurable.
2. Étendre la chaîne d’intégration continue afin qu’elle exécute systématiquement les tests firmware et gateway, Pylint et l’analyse statique.
3. Faire relire les prochaines modifications par une autre personne et séparer les changements fonctionnels, MISRA et documentation en MR distinctes.

