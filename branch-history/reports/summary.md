| seq | commit | non-test +/- | test +/- | files | depends |
|-----|--------|--------------|----------|-------|---------|
| 100 | 0100-upstream-bugfixes | +7/-2 | +0/-0 | 1 | -- |
| 110 | 0110-typeid-evaluated-operand | +24/-7 | +82/-0 | 10 | -- |
| 120 | 0120-test-filter-tool-names | +0/-0 | +1/-2 | 2 | -- |
| 130 | 0130-param-cv-from-definition | +36/-6 | +186/-6 | 8 | -- |
| 140 | 0140-constexpr-destroyed-object | +82/-0 | +166/-0 | 6 | -- |
| 150 | 0150-constexpr-member-before-construction | +85/-0 | +73/-0 | 6 | -- |
| 160 | 0160-float-literal-exponent-range | +9/-0 | +48/-0 | 6 | -- |
| 170 | 0170-source-location-dead-default-arg | +23/-1 | +48/-0 | 5 | -- |
| 180 | 0180-c23-va-start | +96/-10 | +69/-0 | 8 | -- |
| 190 | 0190-nested-this-capture-of-copy | +64/-15 | +48/-0 | 9 | -- |
| 195 | 0195-lambda-pack-capture-leading-ellipsis | +41/-0 | +101/-10 | 11 | -- |
| 500 | 0500-gxx-driver | +267/-0 | +0/-0 | 2 | -- |
| 520 | 0520-gxx-test-config | +279/-4 | +86/-2 | 20 | 0500-gxx-driver |
| 1000 | 1000-p2900-base-basic | +137/-0 | +0/-0 | 10 | -- |
| 1020 | 1020-p2900-base-il | +430/-5 | +0/-0 | 13 | 1000-p2900-base-basic |
| 1040 | 1040-p2900-base-parse | +1290/-6 | +1782/-0 | 83 | 1000-p2900-base-basic, 1020-p2900-base-il |
| 1060 | 1060-p2900-base-sema-core | +574/-0 | +530/-0 | 37 | 1040-p2900-base-parse |
| 1070 | 1070-p2900-base-sema-predicate-scope | +2236/-24 | +1872/-0 | 103 | 1040-p2900-base-parse |
| 1080 | 1080-p2900-base-sema-templates | +541/-24 | +452/-0 | 62 | 1040-p2900-base-parse |
| 1090 | 1090-p2900-base-sema-constexpr | +1134/-24 | +1200/-0 | 64 | 1040-p2900-base-parse |
| 1100 | 1100-p2900-base-lowering | +1336/-1 | +3128/-0 | 268 | 1040-p2900-base-parse |
| 1120 | 1120-p2900-base-il-io | +0/-0 | +641/-0 | 38 | 1090-p2900-base-sema-constexpr, 1100-p2900-base-lowering |
| 1140 | 1140-p2900-base-driver | +47/-0 | +0/-0 | 3 | 0520-gxx-test-config, 1100-p2900-base-lowering |
| 1180 | 1180-p2900-base-tests | +0/-0 | +50123/-0 | 1269 | 1140-p2900-base-driver |
| 2400 | 2400-p3595 | +1708/-0 | +4641/-0 | 284 | 1090-p2900-base-sema-constexpr, 1100-p2900-base-lowering, 1140-p2900-base-driver |
| 2900 | 2900-umbrella-option | +51/-0 | +1117/-0 | 39 | 1000-p2900-base-basic, 1140-p2900-base-driver |
| 3000 | 3000-p3097 | +369/-6 | +6494/-0 | 273 | 2900-umbrella-option |
| 3300 | 3300-p3290 | +69/-0 | +1670/-0 | 150 | 2900-umbrella-option |
| 3600 | 3600-p3099 | +168/-0 | +1274/-0 | 111 | 2900-umbrella-option |
| 4000 | 4000-p3098 | +1373/-8 | +11143/-0 | 565 | 2900-umbrella-option |
| 4200 | 4200-p3400-core | +631/-15 | +17299/-0 | 836 | 2900-umbrella-option |
| 4210 | 4210-p3400-facets | +603/-0 | +175/-0 | 20 | 4200-p3400-core |
| 4220 | 4220-p3400-groups | +139/-0 | +42/-0 | 13 | 4210-p3400-facets |
| 4600 | 4600-p4283 | +626/-0 | +3410/-0 | 174 | 4220-p3400-groups |
| 4800 | 4800-p4298 | +361/-0 | +1886/-0 | 174 | 4600-p4283 |
| 4810 | 4810-p4298-c-gen | +328/-0 | +306/-0 | 64 | 4800-p4298 |
| 4900 | 4900-p4405 | +2627/-0 | +33812/-0 | 654 | 4810-p4298-c-gen |
| 4940 | 4940-p4405-override | +412/-0 | +4687/-0 | 130 | 4900-p4405 |
| 5000 | 5000-p4299 | +103/-0 | +472/-0 | 52 | 4900-p4405 |
| 5200 | 5200-p4301 | +63/-0 | +327/-0 | 37 | 5000-p4299 |
| 6000 | 6000-p3100-core | +307/-0 | +1931/-0 | 161 | 5200-p4301 |
| 6900 | 6900-cross-paper-options | +7/-0 | +0/-0 | 2 | 6000-p3100-core |
| 6990 | 6990-contract-violation-information | +87/-0 | +1390/-0 | 116 | 6900-cross-paper-options, 4940-p4405-override |
| 9900 | 9900-fork-metadata | +6470/-0 | +434/-0 | 148 | -- |
| 9999 | 9999-unknown | +0/-0 | +0/-0 | 0 | -- |
