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
2. **Context remaining percentage** - Free context window space

Format: `main | free:81.0%`

When context usage data is unavailable (after `/clear` or session restart): `main | free:...`

Implementation:
- Status line script at `.claude/scripts/statusline.sh`
- Uses Python for JSON parsing (jq not available in standard Git Bash)
- Reads workspace, context, and cost data from Claude Code's stdin JSON

### Field Justifications

**Git Branch:**
- Prevents commits to wrong branch (especially important with git flow)
- Immediate awareness when switching between feature/main branches
- Low overhead - simple git command

**Context Remaining Percentage (`free`):**
- Supports SST-Claude token economy strategy (ADR-0014)
- Shows percentage remaining/free (matches `/context` output format)
- Enables proactive decisions: compact before running out, or start fresh session
- Critical for Opus model where context is expensive
- Displays `free:...` when usage data unavailable (after `/clear` or restart)

### TBD: Session and Weekly Budget

Session time and weekly budget (as shown by `/status` and `/usage` commands) are not currently exposed to status line scripts. The required data is not available in the JSON passed to status line commands. When Claude Code exposes this data, the format may be extended to include these metrics.

## Consequences

### Positive
- Consistent status line across all project contributors
- Immediate awareness of git branch prevents branch-related mistakes
- Context utilization visibility supports token-conscious workflow
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
