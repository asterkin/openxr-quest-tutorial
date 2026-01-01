# ADR-0011: Use Context7 MCP for Documentation Access

**Status:** Accepted

**Date:** 2025-12-21

## Context

AI assistants have training data cutoffs that cause outdated knowledge about rapidly evolving tools and APIs. This repository uses C++ 20, NDK 29, Gradle 8.x, OpenXR, and Meta XR SDK—all of which evolve faster than training cycles.

Relying on stale training data leads to incorrect code suggestions, deprecated API usage, and missed features. Manual documentation lookup breaks the AI-assisted workflow.

## Decision

Use Context7 MCP (Model Context Protocol) to provide real-time documentation access for all tools and SDKs used in this repository.

Implementation via Claude Code skills (`doc-query`, `add-doc`) with TOML-based configuration, following the pattern established in [claude-taew-py](https://github.com/asterkin/claude-taew-py).

## Consequences

### Positive
- Always-current documentation without waiting for model updates
- Version-specific information for exact SDK versions used
- Consistent workflow—documentation queries stay in-context

### Negative
- External service dependency (Context7)
- Requires API key setup for contributors
- Network connectivity required for documentation queries

### Neutral
- Skills execute locally via Python scripts
- Configuration maintained in `.claude/doc-sources.toml`

### Out of Scope: Claude Documentation

Claude Code, Agent SDK, and Anthropic API documentation do **not** require Context7 access. Claude Code includes a built-in `claude-code-guide` subagent that automatically handles documentation queries about:
- Claude Code CLI (hooks, skills, MCP servers, settings)
- Claude Agent SDK (building custom agents)
- Anthropic API (tool use, SDK usage)

This subagent uses web search internally and requires no configuration.

## References
- [Context7](https://context7.com) - Documentation access service
- [Reference implementation](https://github.com/asterkin/claude-taew-py/tree/main/.claude/skills)
- [Phase 7 execution plan](../Project_Plan.md#phase-7-connect-to-context7-mcp)
