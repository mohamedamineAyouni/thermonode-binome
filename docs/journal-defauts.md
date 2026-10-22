# Journal des défauts révélés — TP 3

| ID | Étape | Exigence | Test révélateur | Symptôme observé (message Unity/pytest) | Hypothèse de cause | Technique qui l'a révélé |
|---|---|---|---|---|---|---|
| DEF-01 | 1 | EF-05 | test_EF05_range_check_bounds | Expected TRUE Was FALSE pour 12500 | borne haute exclue (< au lieu de <=) | BVA |
| DEF-02 | 2 | EF-03 | test_EF03_transient_partial_window; test_EF03_invalid_sample_is_excluded | Expected 100 Was 2600; dépassement mémoire ligne 30 | accès à l index 5 hors tableau | tests de séquence et AddressSanitizer |
| DEF-03 | 4 | EF-04 | test_EF04_counter_resets_below_threshold | Expected 0 Was 1 après 7000,7000,2500,7000,7000 | compteur au-dessus du seuil non réinitialisé | table de décision |
| DEF-06 | 5 | EF-05 | test_EF05_recovers_to_normal_after_5_valid_samples | Expected 1 Was 3 après 5 mesures valides | transition de reprise absente ou incorrecte dans node.c | test de transition d état avec horloge simulée |
| DEF-04 | 6 | EI-02 | test_EI02_crc_check_value_123456789_is_29B1; test_EI02_crc_of_empty_is_FFFF | Valeurs attendues 0x29B1 et 0xFFFF, obtenues 0x31C3 et 0x0000 | valeur initiale du CRC incorrecte | oracle externe CRC normalisé |
