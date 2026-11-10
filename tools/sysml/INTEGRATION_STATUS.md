# SysML workflow integration: implementation and verification

The default frontend and experiment workflow now uses the real reference validator and deterministic compiler. Usage and executable-language boundaries are documented in PIPELINE.md and PIPELINE_DETAILED.md.

## Requirement audit

- Repository cleanup: removed 164 regenerable preview PNGs, the unused pseudo-SysML renderer and obsolete LLM-C prompt assembly helpers. Retained user documents, experiment results and both preexisting untracked datasets.
- GitHub tool: installed unmodified SysML Pilot Implementation 2026-08 / 0.62.0 with matching standard libraries and retained EPL-2.0 license. Archive SHA256 is verified by installer. Private Java 21 leaves system Java unchanged.
- LLM feedback: real syntax/reference/semantic diagnostics and compiler diagnostics feed a bounded repair loop; malformed bundles and exhausted retries return errors. Tool failures are separate from model failures.
- Compiler: official semantic AST -> deterministic scalar/modular graph IR; state/action graph extension -> existing IL XML. Nested/repeated instances, unique-driver bindings, conditionals, initialization, conditional and explicitly ordered state updates are supported. Unsupported language features fail explicitly, as detailed in PIPELINE_DETAILED.md.
- Toolchain: actual installed ILTranslator emits C. Frontend, experiment generator and old C-generation compatibility endpoint all use the compiler; experimental fallback to LLM C was removed.
- Arduino: frontend/experiments pass the generated header/source as compiled_kernel; the implementation fixes that kernel into the sketch and requests only hardware glue from the LLM. Structural adapter failures are retried. Integer ABI is kept 32-bit on AVR.
- Incremental context: saved/restored system state includes actual SysML and requirement mapping.

## Verification evidence

- Final full sysml*unittest.py run passed 13 tests in 177.646 seconds, covering actual reference tooling, API diagnosis/repair, modular isolation, conditional expressions, array rejection, adapter isolation and real ILTranslator/g++ execution of stateful models.
- Compatibility endpoint test is included: generation of C makes no LLM call and produces the expected square computation.
- Stateful generated C++ was executed: count 1, 2, reset to 0, then 1; explicit initialization resets correctly. Ordered updates execute a=a+1 before b=a+2, yielding 3 then 4.
- Frontend tsc/Vite production build passed with compiled_kernel request included.
- Python syntax compilation passed. No real paid LLM calls or physical hardware tests were made; LLM responses in automated tests are scripted substitutes.

## Operational boundaries

This is an executable SysML subset, not a complete compiler for every standard feature or a proof of requirement/hardware correctness. General state machines, concurrency, arbitrary loops, arrays, inheritance and instance overrides are rejected. The sampled-step and numeric contracts are documented. Target-board compilation and device behavior remain project-specific validation, not claimed by host tests.

The current installation lacks ParserForTBLink.exe and ILGenerator.exe, but has ILTranslator.exe. The default route emits IL directly; it does not depend on those absent programs. Stateful graphs are not exported as misleading stateless TBLink.

The old .venv is cross-platform and unusable on Windows; the available Windows Python runs the application and tests. Downloaded tools live under ignored .tools/sysml. Tests and implementation files are intentionally left uncommitted for user review.

## Final edit-path regression audit

- Arduino edit regression: 6 tests pass, including preserving literal backslashes, rejecting injected parameter statements/invalid UNO pins, and refusing edits to the compiled SysML kernel before saving.
- Existing Arduino static-quality suite: 68 tests pass.
- Existing code_seed_unittest.py: 5 tests still fail (2 absent _clean_code_seed function references, 1 absent frontend-new/src/App.tsx path, and 2 outdated prompt-text assertions). These are baseline issues: HEAD also lacks the function/path, and the two prompts and test file were unchanged by this work. This legacy suite is not represented as passing.
- Editor checks are structural; they do not establish hardware correctness or semantic equivalence of arbitrary adapter edits.

## 2026-09-26 UI, repair, and real model experiment

- Frontend adds complete module descriptions with upstream/downstream context; the blueprint prompt requests concrete input/processing/output/constraint details. Existing descriptions are preserved when code is generated or viewed. Old saved descriptions are retained rather than invented; replanning generates fuller text.
- SysML API exposes per-attempt diagnostics and repair counts. The frontend waits up to 20 minutes for this endpoint, displays the trace, and stops downstream work on exhausted retries. Complete JSON markdown fences are normalized before validation; the real validator/compiler gates remain mandatory.
- 15 SysML tests passed in 77.064 seconds. Final frontend production build and browser dialog open/close inspection passed. Vite's local fallback proxy is now port 8001 because Windows reserves 8000.
- Real GPT-4o-mini pilot: 12 fixed samples across internal/PCEval/EmbedBench, 60 paired arm records, 48 actual calls, 113160 tokens, reported API cost $0.0293991 (excluding preflight/interrupted development pilot). Direct generation compiled 12/12; workflow compiled 0/12 because all three model attempts failed. Deterministic compiler ablation was blocked by the shared model prerequisite, not evidence about compiler quality.
- Full report: experiment/output/controlled_pilot_v2_20260926/REPORT_ZH.md. Design and caveats: experiment/CONTROLLED_EXPERIMENT.md. No Wokwi/hardware functionality was tested. These results do not establish a workflow advantage.

## 2026-10-08 repair verification (in progress)

- SysML compiler now emits the strict IR v2 contract for both stateless and stateful models. Stateless TBLink export accepts empty-state v2; stateful TBLink export remains rejected to avoid losing behavior.
- Failed SysML candidates are regenerated with the original requirements, previous complete candidate, actual diagnostics and targeted repair guidance. Every replacement is validated again. Exhausted repair budgets still stop downstream generation; tool failures remain infrastructure errors.
- Verified: `python -m unittest discover -s tests -p 'sysml*unittest.py' -v` passed 18 tests in 210.088 seconds. Coverage includes the real reference validator, strict IR checks for four models, API repair/saving behavior, real ILTranslator and host execution/reset of stateful kernels.
- Independent development evaluation is running through `experiment/code/run_development_repair.py`: all 60 preassigned development tasks, one repetition, six existing arms, GPT-4o-mini and unchanged per-stage repair budgets. Output: `experiment/output/development_repair_20261008`. This is development evidence, not a new held-out formal evaluation; compilation does not establish functional correctness.
- Original formal records and method snapshots remain untouched. No claim of measured generation-quality improvement is made until this development run is inspected. No Wokwi or hardware functionality is claimed for this run.
