# ADR-0004: Establish Open AI Code Assistants Structure

**Status**: Accepted

## Context

AI code assistants can significantly reduce onboarding time for developers learning OpenXR Quest development. However, developers may have different preferences and subscriptions:

- **Claude Code** (Anthropic)
- **GitHub Copilot** / **Codex** (OpenAI)
- **Gemini Code Assist** (Google)
- **Grok** (xAI)
- Future AI assistants

Each AI assistant requires different integration approaches:
- Different entry point file conventions
- Different context mechanisms
- Different extension systems

### The Challenge

How do we structure repository documentation to support multiple AI assistants without:
- Duplicating content across agent-specific files
- Creating maintenance burden
- Fragmenting documentation
- Locking into a single AI vendor

## Decision

We will establish an **open AI code assistants structure** with agent-neutral documentation and minimal agent-specific entry points.

### Architecture Principles

1. **Agent-Neutral Core**: All significant project documentation in standard Markdown format
2. **Minimal Entry Points**: Agent-specific context files at repository root (e.g., `CLAUDE.md`, `COPILOT.md`)
3. **Token-Efficient Guidance**: Entry point files provide navigation guidance, not content duplication
4. **Agent-Specific Extensions**: Optional agent features in dedicated folders (e.g., `.claude/`, `.github/copilot/`)

### Repository Structure

```
openxr-quest-tutorial/
├── CLAUDE.md                    # Claude Code entry point (initial focus)
├── COPILOT.md                   # GitHub Copilot entry point (future)
├── GEMINI.md                    # Gemini Code Assist entry point (future)
├── README.md                    # Human-readable overview
│
├── docs/                        # Agent-neutral documentation
│   ├── adrs/                    # Architecture Decision Records
│   ├── Environment_Setup.md     # Installation guide
│   ├── Build_Deploy_Guide.md    # Build workflows
│   ├── Project_Plan.md          # Architecture and phases
│   └── Repository_Structure.md  # Directory layout
│
├── .claude/                     # Claude Code-specific extensions
│   ├── skills/                  # Claude Code skills (if needed)
│   └── commands/                # Slash commands (if needed)
│
├── .github/                     # GitHub-specific (future)
│   └── copilot/                 # Copilot instructions (future)
│
└── samples/                     # Sample projects (agent-neutral)
    └── hello_xr/
        └── README.md            # Standard Markdown
```

### Entry Point File Responsibilities

Agent-specific files (e.g., `CLAUDE.md`) should:
- **Provide navigation**: Point to relevant documentation in `docs/`
- **Define working style**: Token efficiency, response format, interaction patterns
- **Reference, not duplicate**: Link to detailed docs, don't copy content
- **Stay lightweight**: Minimize token usage in agent context

**Example pattern**:
```markdown
# CLAUDE.md

## Quick Reference
- Environment Setup: [docs/Environment_Setup.md](docs/Environment_Setup.md)
- Build Guide: [docs/Build_Deploy_Guide.md](docs/Build_Deploy_Guide.md)
- Architecture: [docs/Project_Plan.md](docs/Project_Plan.md)

## Navigation Guidelines
When user asks about X, refer to docs/Y.md
```

## Consequences

### Advantages

**For Developers**:
- Freedom to choose preferred AI assistant
- Consistent documentation experience across agents
- No vendor lock-in

**For Maintainers**:
- Single source of truth (`docs/` directory)
- Minimal duplication reduces maintenance burden
- Easy to add support for new AI assistants

**For AI Assistants**:
- Clear entry point for context
- Token-efficient navigation (references, not duplication)
- Agent-neutral documentation works with all LLMs

### Disadvantages

- Must maintain multiple entry point files (one per supported agent)
- Agent-specific optimizations require understanding each AI's context mechanisms
- Initial setup overhead to establish structure

### Mitigations

- Start with Claude Code only (Phase 1)
- Add support for other agents based on demand
- Document entry point file conventions in `docs/AI_Assistants_Guide.md` (future)

### Anti-Patterns to Avoid

❌ **Duplicating content** across agent files:
```markdown
# CLAUDE.md - BAD
## Environment Setup
Install Java 17, Android SDK Platform 34...
(duplicates docs/Environment_Setup.md)
```

✅ **Referencing documentation**:
```markdown
# CLAUDE.md - GOOD
## Environment Setup
See [docs/Environment_Setup.md](docs/Environment_Setup.md)
```

❌ **Agent-specific documentation in `docs/`**:
```markdown
# docs/Claude_Guide.md - BAD
Claude-specific instructions mixed with project docs
```

✅ **Agent-neutral `docs/`, agent-specific root files**:
```markdown
# docs/Environment_Setup.md - GOOD
Standard Markdown documentation usable by any agent or human
```

## References

- [CLAUDE.md](../../CLAUDE.md) - Current Claude Code entry point
- [Repository Structure](../Repository_Structure.md) - Directory layout
- [ADR-0001](adr-0001-consolidated-tutorial-repository-for-openxr-quest-samples.md) - Tutorial repository rationale
