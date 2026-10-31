# RAFCODE freestanding — reconciliação do HEAD (31/10/2026)

> DRAFT/HOLD · leitura estática. `rafaelmeloreisnovo/termux-packages-rafcode@e9699f46308c199c06f675b56ee7295a7b61b05e` (`main`). **Correção de documentação:** o merge da PR #1 ocorreu na `main`; os predecessores [`README.md`](../README.md) e [`CONTRACT.md`](CONTRACT.md) descrevem a situação anterior ao merge. Merge é incorporação de fontes, não aprovação científica/CI/device.

Este é um **experimento C11 freestanding isolado de propagação potencial em grafo**, e não a fábrica `termux-packagesMy`, um pacote APT, um novo Android SDK/NDK ou um substituto de `pkg/proot`.

## Fontes e escopo

- Código: [`include/raf_cascade_v1.h`](../include/raf_cascade_v1.h), [`src/raf_cascade_v1.c`](../src/raf_cascade_v1.c), [`tests/test_cascade_v1.c`](../tests/test_cascade_v1.c).
- CI declarada: [`.github/workflows/freestanding-ci.yml`](../.github/workflows/freestanding-ci.yml). O YAML compila objetos `x86_64-linux-gnu`, `armv7a-linux-gnueabi`, `aarch64-linux-gnu`, pede `nm -u` vazio e prevê **22 assertions hosted** no runner Linux; **não atesta que executaram neste HEAD**.
- A CI atual é disparada por push na branch antiga `work/freestanding-risk-cascade-v1-20261031` e por PRs com paths filtrados. **Push direto na main não está coberto por esse gatilho**.
- Política lógica (contrato): `CONFIRMED`, `UNKNOWN` e `CUT`; `UNKNOWN` alcançável bloqueia, `TOKEN_VAZIO != 0`. Dimensões D6/D9/D12/D18 são rótulos, não modelos físicos.

## Gate e lacunas

`SOURCE != ARTIFACT != EXECUTION != EVIDENCE != CLAIM`. A confirmação do merge #1 e a presença de fontes/CI não equivalem a executável freestanding validado, rodada exata do CI, APK construído, pacote instalado ou execução ARM32/ARM64 em celular. `claim_allowed=false`.

**F_ok:** HEAD, arquivos e workflow atuais identificados. **F_gap:** CI no SHA exato, assertions e falsificadores observados, comportamento em dispositivo e integração com fábrica de pacotes = `TOKEN_VAZIO_NOT_RUN` nesta revisão. **F_next:** validar na CI exata da branch de documentação ou num PR de código posterior; não ampliar claim. **Rollback:** fechar PR documental sem merge.
