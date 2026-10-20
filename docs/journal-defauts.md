# Journal des défauts révélés — TP 3

| ID | Étape | Exigence | Test révélateur | Symptôme observé (message Unity/pytest) | Hypothèse de cause | Technique qui l'a révélé |
|---|---|---|---|---|---|---|
| DEF-01 | 1 | EF-05 | test_EF05_range_check_bounds | Expected TRUE Was FALSE pour 12500 | borne haute exclue (< au lieu de <=) | BVA |
| DEF-02 | 2 | EF-03 | test_EF03_transient_partial_window; test_EF03_invalid_sample_is_excluded | Expected 100 Was 2600; dépassement mémoire ligne 30 | accès à l index 5 hors tableau | tests de séquence et AddressSanitizer |
