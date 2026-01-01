# ADR-0016: Project-Level Status Line Configuration

**Status:** Accepted

**Date:** 2026-01-01

## Context

Claude Code provides a customizable status line that displays information during interactive sessions. This project uses a token-conscious workflow (SST-Claude strategy, see ADR-0014) where awareness of context window utilization is critical for making informed decisions about when to compact or start fresh sessions.

Additionally, this repository will implement git flow for branch management. Knowing the current git branch at a glance helps prevent mistakes like committing to the wrong branch.

The status line can be configured at user level (`~/.claude/settings.json`) or project level (`.claude/settings.json`). Project-level configuration ensures consistent experience across team members and machines.

## Decision

Configure a project-level status line in `.claude/settings.json` that displays:

1. **Current git branch** - Shows which branch is checked out
2. **Context free percentage** - Remaining context window capacity
3. **Session time percentage** - Time used of 5-hour session limit

Format: `main | ctx:87% ses:12%`

Implementation:
- Status line script at `.claude/scripts/statusline.sh`
- Uses Python for JSON parsing (jq not available in standard Git Bash)
- Reads workspace, context, and cost data from Claude Code's stdin JSON

### Field Justifications

**Git Branch:**
- Prevents commits to wrong branch (especially important with git flow)
- Immediate awareness when switching between feature/main branches
- Low overhead - simple git command

**Context Free Percentage (`ctx`):**
- Supports SST-Claude token economy strategy (ADR-0014)
- "Free" rather than "used" because remaining capacity is the actionable metric
- Enables proactive decisions: compact before running out, or start fresh session
- Critical for Opus model where context is expensive

**Session Time Percentage (`ses`):**
- Pro subscription has 5-hour session time limit
- Even token-cheap operations consume session time
- Awareness prevents unexpected session exhaustion mid-task
- Calculated from `cost.total_duration_ms` / 18,000,000ms (5h)

### TBD: Weekly Budget

Weekly budget (as shown by `/usage` command) is not currently exposed to status line scripts. When Claude Code exposes this data, the format will be extended to: `main | ctx:87% ses:12% wk:45%`

## Consequences

### Positive
- Consistent status line across all project contributors
- Immediate awareness of git branch prevents branch-related mistakes
- Context utilization visibility supports token-conscious workflow
- Session time awareness prevents mid-task exhaustion of 5h limit
- Configuration versioned with project, not lost on machine changes

### Negative
- Requires Python in PATH (standard for this project)
- Script executed on each status update (every ~300ms) - minimal overhead
- Project-level config overrides any user preferences for status line

### Neutral
- Status line format is opinionated but can be modified in script
- Other projects on same machine unaffected (project-level scope)

## References
- [ADR-0014: Disable Auto-Compact for Token Economy](adr-0014-disable-auto-compact-for-token-economy.md)
- [ADR-0015: Project-Level Claude Permissions](adr-0015-project-level-claude-permissions.md)
- [Claude Code Status Line Documentation](https://docs.anthropic.com/en/docs/claude-code/settings#status-line)
- SST-Claude strategy: `docs/claude-sst.md`
