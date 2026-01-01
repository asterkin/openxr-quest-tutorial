# ADR-0014: Disable Auto-Compact for Token Economy

**Status:** Accepted

**Date:** 2026-01-01

## Context

As part of SST-Claude token economy optimization (see `docs/claude-sst.md`), we analyzed Claude Code's context window management. The auto-compact feature reserves ~22.5% of the context window as a buffer for automatic compaction when context approaches capacity.

With auto-compact enabled:
- 22.5% of context is reserved (unavailable for working memory)
- Compaction triggers automatically at ~95% capacity
- System decides what to summarize (lossy, uncontrolled)

With auto-compact disabled:
- Full context window available for working memory
- Manual `/compact <focus>` gives control over what to preserve
- Requires discipline to monitor context usage via `/context`

For this tutorial repository focused on learning SST patterns and token optimization, manual memory management aligns with our experimental goals.

## Decision

Disable auto-compact for this project via `/config` toggle.

**Current limitation:** The `autoCompactEnabled` setting is stored in `~/.claude.json` (user-level), not project-level. This means:
- The setting affects **all projects** on the workstation, not just this one
- There is no per-project configuration for auto-compact
- A PreCompact hook can block the operation but cannot reclaim the buffer

We accept this limitation and document the recommended configuration rather than implement workarounds.

## Consequences

### Positive
- ~22.5% more working context available (~45k tokens on 200k window)
- Manual `/compact <focus>` preserves intentionally chosen context
- Forces explicit memory management decisions (learning opportunity)
- Aligns with SST goal of understanding token economics

### Negative
- Affects all Claude Code projects on the workstation
- Risk of hitting context wall mid-task if not monitoring
- Requires manual intervention to compact before capacity
- Team members must each configure their own workstation

### Neutral
- `/context` command becomes essential for monitoring usage
- Workflow shifts from reactive (auto) to proactive (manual) memory management

### Implications for Session Planning

All session activities (SST graph path traversals) must be planned to fit within available context memory without compaction. This means:
- Estimating token cost of planned operations before starting
- Breaking large tasks into context-sized chunks across sessions
- Using `/compact <focus>` strategically between task phases, not as emergency recovery

**Future enhancements (TBD):**
- Virtual context memory (paging context to/from external storage) — technically feasible, unclear if practical benefit justifies complexity
- Sub-agent delegation for context isolation — already supported, needs SST patterns for optimal use

## References
- [SST-Claude research document](../claude-sst.md) — Token economy optimization framework
- [Claude Code Memory documentation](https://code.claude.com/docs/en/memory.md)
- [Claude Code Hooks documentation](https://code.claude.com/docs/en/hooks.md#precompact)
- GitHub discussions on auto-compact limitations (community feedback pending resolution)
