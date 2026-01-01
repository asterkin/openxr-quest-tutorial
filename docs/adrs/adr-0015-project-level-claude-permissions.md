# ADR-0015: Project-Level Claude Permissions

**Status:** Accepted

**Date:** 2026-01-01

## Context

Claude Code requires explicit permission for potentially sensitive operations (shell commands, web access, skill invocation). These permissions accumulate in `.claude/settings.local.json` as ad-hoc approvals during sessions.

Over time, this file became cluttered with:
- Duplicate permissions (e.g., multiple git subcommands instead of `git:*`)
- Path-specific commands tied to absolute paths (brittle across machines)
- Windows-specific commands (obsolete in Git Bash environment)
- Redundant entries (e.g., `python:*` plus specific python scripts)

Additionally, `settings.local.json` is user-specific (gitignored per ADR-0014), meaning each team member must re-approve the same operations.

## Decision

Consolidate permissions into `.claude/settings.json` (project-level, committed to git) with a minimal, coherent rule set organized by category:

### Git Operations
- `Bash(git:*)` — All git commands

### Build Operations
- `Bash(./gradlew:*)` — Unix-style Gradle wrapper
- `Bash(gradlew.bat:*)` — Windows Gradle wrapper

### Script Execution
- `Bash(python:*)` — Python scripts (project automation, skills)

### File Utilities
- `Bash(cp:*)` — Copy files
- `Bash(ls:*)` — List directory contents
- `Bash(wc:*)` — Word/line count
- `Bash(unzip:*)` — Extract archives
- `Bash(curl:*)` — HTTP requests (API testing, downloads)

### Web Access
- `WebFetch(domain:github.com)` — GitHub repository access
- `WebFetch(domain:raw.githubusercontent.com)` — Raw GitHub content
- `WebFetch(domain:context7.com)` — Context7 documentation API
- `WebSearch` — Web search for research

### Skills
- `Skill(adr)` — ADR creation skill

### Guarded Operations (ask)
- `Bash(rm:*)` — File/directory deletion (requires explicit confirmation each time)

## Consequences

### Positive
- Permissions shared across team (no per-user re-approval)
- Reduced from 26 ad-hoc rules to 14 coherent rules
- Portable across machines (no absolute paths)
- Self-documenting (organized by category in this ADR)

### Negative
- Broader patterns (e.g., `git:*`) grant more access than specific commands
- Team must agree on permission scope (security vs. convenience trade-off)

### Neutral
- `settings.local.json` can still override for user-specific needs
- New permission categories require ADR update (intentional friction)

## References
- [ADR-0014](adr-0014-disable-auto-compact-for-token-economy.md) — Moved settings.local.json to gitignore
- [Claude Code Settings documentation](https://code.claude.com/docs/en/settings.md)
