# Journal des défauts révélés — TP 3

| ID | Étape | Exigence | Test révélateur | Symptôme observé (message Unity/pytest) | Hypothèse de cause | Technique qui l'a révélé |
|---|---|---|---|---|---|---|
| DEF-01 | 1 | EF-05 | test_EF05_range_check_bounds | Expected TRUE Was FALSE pour 12500 | borne haute exclue (< au lieu de <=) | BVA |
| DEF-02 | 2 | EF-03 | test_EF03_transient_partial_window; test_EF03_invalid_sample_is_excluded | Expected 100 Was 2600; dépassement mémoire ligne 30 | accès à l index 5 hors tableau | tests de séquence et AddressSanitizer |
| DEF-03 | 4 | EF-04 | test_EF04_counter_resets_below_threshold | Expected 0 Was 1 après 7000,7000,2500,7000,7000 | compteur au-dessus du seuil non réinitialisé | table de décision |
| DEF-06 | 5 | EF-05 | test_EF05_recovers_to_normal_after_5_valid_samples | Expected 1 Was 3 après 5 mesures valides | transition de reprise absente ou incorrecte dans node.c | test de transition d état avec horloge simulée |
| DEF-04 | 6 | EI-02 | test_EI02_crc_check_value_123456789_is_29B1; test_EI02_crc_of_empty_is_FFFF | Valeurs attendues 0x29B1 et 0xFFFF, obtenues 0x31C3 et 0x0000 | valeur initiale du CRC incorrecte | oracle externe CRC normalisé |
| DEF-05 | 7 | ENF-05 | test_ENF05_push_61_keeps_60_and_drops_oldest | Expected 60 Was 61 après 61 insertions | le tampon conserve un élément de trop au lieu de rester à 60 | test de frontière 59/60/61 |
| DEF-07 | 9 | EI-02 | test_EI02_invalid_frames_are_rejected[temp_string]; test_EI02_invalid_frames_are_rejected[seq_negative] | DID NOT RAISE FrameError pour temp chaîne et seq=-1 | validation trop permissive des champs temp et seq | tests paramétrés, une anomalie par cas |
| DEF-08 | 9 | ENF-05 | test_ENF05_duplicate_seq_is_ignored; test_ENF05_out_of_order_old_frame_is_ignored; test_last_seq_survives_restart | doublon compté à 0 au lieu de 1; ancienne trame stockée; doublon accepté après redémarrage | dernier numéro de séquence non mémorisé ou non vérifié correctement | tests unitaires avec base temporaire et redémarrage |
