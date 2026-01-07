# Codex-SST: Practical Semantic Spacetime for Codex CLI

This is a minimal, concrete guide for applying SST ideas to **Codex CLI** work in this repository.
It favors small, repeatable actions over conceptual breadth.

## Goal

Increase task accuracy while minimizing context bloat by:
1) searching before reading,
2) reading only the needed section,
3) expanding scope only when necessary.

## Quick Map: SST -> Codex Actions

| SST Concept | Codex Action | Example |
|-----------|--------------|---------|
| CONTAINS / CONTAINED-IN | Move up/down directory structure | `dir`, `dir /s /b *.md` |
| FOLLOWS / FOLLOWED-BY | Follow task or build flow | Read `docs/Project_Plan.md` |
| EXPRESSES / EXPRESSED-BY | Find descriptions / metadata | `type docs/Repository_Structure.md` |
| NEAR | Co-accessed files signal missing links | Add cross-ref in doc if repeated |

## Minimal Workflow (Default)

1) **Locate**: `rg --files` or `dir /s /b` to find targets.
2) **Narrow**: `rg "pattern"` to find sections or symbols.
3) **Read small**: `type path` only after narrowing.
4) **Act**: apply change in one file at a time.
5) **Verify**: quick re-read of the touched section.

If a step expands scope twice without progress, stop and ask for clarification.

## "Pointer First" Rule

Treat large docs as pointers:
- `docs/Project_Plan.md` -> phase context
- `docs/Repository_Structure.md` -> navigation
- `docs/Environment_Setup.md` -> SDK/NDK setup

Only load the specific file when the task explicitly requires it.

## NEAR Heuristic (Practical)

If you repeatedly read two files together:
- Add a short cross-reference in one of them.
- Prefer a single sentence with a path pointer.

This is the only suggested refactor for SST in this repo.

## When NOT to Use SST

Skip this model when:
- The task is trivial (single file, single change).
- The user request is already precise and scoped.
- You are asked for a direct review or summary.

## Codex-SST Success Signals

You are using this well when:
- You read fewer whole files and more targeted sections.
- You can answer where to look next without reading extra files.
- You can finish tasks with fewer follow-up questions.

## Optional: Lightweight Session Checkpointing

Before a risky change:
1) Restate the goal in one sentence.
2) Name the single file you will edit next.
3) Proceed.

This replaces heavy tracking files (no `CONTEXT.md` required).

## Appendix: Repo-Specific Examples

Examples use Windows `cmd.exe` and assume repo root.

1) Find sample READMEs:
   - `dir /s /b openxr\\*\\README.md`
2) Locate build guidance:
   - `type openxr\\Build_Guidelines.md`
3) Search for OpenXR API usage:
   - `rg "xrCreate|xrEnumerate|xrGet" openxr`
4) Find native build files:
   - `dir /s /b **\\CMakeLists.txt`
5) Narrow to Android/Gradle blocks:
   - `rg "android \\{" -g "*.gradle" -g "*.gradle.kts"`

## Appendix: Review of docs/claude-sst.md

Critical review highlights (ordered by impact):
1) Conceptual breadth exceeds operational guidance; many frames, few concrete steps.
   - Multiple macro-level models (SST graph, VM analogy, RISC, Promise Theory).
   - No minimal checklist for a single task cycle in this repo.
2) Terminology overlaps (SST/VM/RISC/Promise Theory) without enforced mapping.
   - Same ideas re-stated with different terms; hard to know which is primary.
   - Increases ambiguity in “what to do next” during real work.
3) "NEAR" is suggestive but lacks thresholds or repo-specific examples.
   - No rule for when a repeated co-read becomes worth refactoring.
   - No example like “docs/Project_Plan.md <-> docs/Repository_Structure.md”.
4) Token economics are hypothesized without observable instrumentation.
   - Good framing, but no practical proxy (e.g., “number of files read”).
   - Leaves no way to validate wins or detect regressions.
5) New artifacts (CONTEXT.md, scripts) are proposed but not defined or integrated.
   - Introduces maintenance and process risk in a tutorial repo.
   - Without a lightweight spec, agents will diverge in usage.
6) Examples are mostly abstract, not tied to this repo's actual docs/code.
   - This weakens transfer from theory to daily usage.
7) Layering and traversal assume stable “current cursor” state.
   - In practice, agent sessions are discontinuous and user-driven.
   - Without a defined cursor source, this part stays speculative.

Practical takeaway used in this document:
- Keep SST usage minimal, action-first, and grounded in repo paths and commands.
- Prefer visible, low-friction measures (search-first, pointer docs, small reads).
- Avoid introducing new persistent files until a clear workflow proves value.

What to keep:
- The core aim of minimizing context while preserving accuracy.
- The four connection types as a light vocabulary for navigation.
- The “pointer-first” idea for large docs.
- The bottom-up, experience-driven rollout principle.
