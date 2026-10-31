# RAFCODE CASCADE FREESTANDING V1 — DRAFT/HOLD

Source contract: `rafaelmeloreisnovo/termux-packagesMy@c56d34d330ff8ca621c93bbe8c36d6ab52f3844e`, `AGENTS.md` and `docs/04-AUDIT_AND_EVOLUTION.md`. This is independent, original C code; it does not transplant Termux third-party sources or imply Android runtime compatibility.

## Contract
- Deterministic **potential reachability** across directed producer/consumer dependency links, not prediction of physical hazard probabilities.
- Fixed capacity 1–32 nodes, 0–96 links, optionally 1–3 declared participants and bounded reach/depth/frontier budgets. No heap, libc, syscalls or external runtime dependencies in core.
- Relation `CONFIRMED` is a declared topology input; `UNKNOWN` fails closed if reachable; `CUT` excludes a link. The caller supplies proposed cuts; **core never mutates a system**.
- `dimension_label` is an opaque label for D6/D9/D12/D18 or unknown (0), never a mathematical rule. No assumption that `D9` models three-player Mexican Train dominoes or that combinatorial fanout is exponential.
- `SOC` has `TOKEN_VAZIO_SOC_SEMANTICS`: self-organized criticality and security operations center are different domains. `AVALANCHE` / `DOMINÓ` are **scenario labels** requiring typed event sources.
- Decision priority: INVALID > UNKNOWN > CRITICAL > REACH_BUDGET > DEPTH_BUDGET > FRONTIER_BUDGET > ALLOW_SCOPED.
- `rollback_required` is an advisory flag for a separate, authorized deployment layer; it **never executes rollback**.
- A reachable unknown is a blocking state, not value zero. Unknown disconnected from the active seed is not automatically a stop.

## Verification
- Core is freestanding C11 with standard compiler header `stdint.h`, `-ffreestanding -fno-builtin -fno-stack-protector`.
- Host harness is intentionally hosted (uses `main`/platform CRT); this does **not** turn the core into libc-dependent code.
- The CI workflow compiles three objects for x86_64 / ARMv7a / AArch64, verifies `nm -u` is empty, inspects ELF architecture, and runs 22 bounded negative/positive contract assertions on the x86_64 host.
- Hosted runner execution ≠ Android device execution ≠ Termux package installation. CI runner needs Clang and GNU binutils preinstalled; no SDK/NDK/docker requirement.
- Source origin pinned above; original `termux-packagesMy` and Android app files unchanged.

## Risk dimensions and unknowns
`TOKEN_VAZIO`: real fault propagation direction/causality, semantic Dn rules, criticality weights, third-party provenance for external adaptation, load/failure distribution, real package DAG fixture, quantification of false positives, Android ARM32/ARM64 runtime, hardware test, and independently signed receipt.

**Gate:** `SOURCE != ARTIFACT != EXECUTION != EVIDENCE != CLAIM`. `claim_allowed=false`.

## Reversibility
The repository originally had no commits. `main` received one minimal README to enable PR review; all new source/CI files are confined to `work/freestanding-risk-cascade-v1-20261031`, with **no merge**. To roll back the experiment: close the draft PR or move the work branch after independent review, without deleting the minimal bootstrap.
