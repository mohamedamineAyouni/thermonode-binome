# Dérogations MISRA C:2012 — ThermoNode

| ID | Règle | Fichier | Justification | Approbation |
|---|---|---|---|---|
| DEV-001 | 11.4 (conversion pointeur ↔ entier) | hal/hal_stm32.c (via STM32Cube) | Accès aux registres mappés en mémoire par la bibliothèque HAL du fabricant | Responsable qualité |
| DEV-002 | 21.6 (`<stdio.h>`) | src/frame.c, src/node.c | `snprintf` utilisé pour la sérialisation JSON avec contrôle de la longueur ; alternative (sérialisation manuelle) jugée plus risquée | Responsable qualité |
| DEV-003 | 21.6 (`<stdio.h>`) | hal/hal_sim.c | Code de simulation hôte, non embarqué | — |

| DEV-004 | 15.5 | src/* | Les retours anticipés servent à rejeter les entrées invalides et à protéger les opérations suivantes. | Responsable qualité |
| DEV-005 | 18.4 | src/frame.c | Le parseur manipule des pointeurs dans les buffers JSON après vérification des longueurs et capacités. | Responsable qualité |
| DEV-006 | 21.1, 21.10 | src/main.c | Les éléments POSIX concernés servent uniquement à la simulation sur l’hôte et ne sont pas utilisés dans le firmware embarqué. | Responsable qualité |
