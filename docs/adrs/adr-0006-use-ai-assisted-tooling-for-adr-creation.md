# ADR-0006: Use AI-Assisted Tooling for ADR Creation

**Status**: Accepted - Phase 1 (Skill) - 2025-12-05

## Context

Architecture Decision Records (ADR-0000) provide significant value by documenting the "why" behind architectural decisions. However, creating and maintaining ADRs introduces overhead that can hinder adoption:

### Manual ADR Creation Challenges

1. **Token Waste**: AI agents spend tokens on mechanical tasks (formatting, numbering, TOC updates) that scripts could handle
2. **Inconsistency**: Manual creation leads to formatting variations and incomplete metadata
3. **Adoption Friction**: Contributors may skip ADRs if the process feels cumbersome
4. **Context Loss**: Without templates and guidance, ADRs may miss critical sections
5. **Maintenance Burden**: Keeping TOC synchronized with ADR files requires manual effort

### AI-Native Philosophy Alignment

This project follows an AI-native approach (ADR-0004: Open AI Code Assistants Structure). The same principle applies to ADR tooling:
- AI agents should assist with ADR creation, not just consume them
- Tooling should reduce token waste by delegating mechanical tasks
- The meta-tooling decisions themselves deserve ADR treatment (meta-bootstrapping)

### Dual Purpose of ADRs in This Project

ADRs serve two audiences:
1. **Human contributors**: Understanding historical context and rationale
2. **AI agents**: Answering "why" questions about architecture decisions

This dual purpose makes AI-assisted ADR creation particularly valuable—AI helps create the knowledge base it will later consume.

## Decision

Implement AI-assisted tooling for ADR creation and maintenance using a **phased evolution approach**, starting simple and adding capabilities based on experience.

### Phase 1 (Current): Skill-Based Guidance

**Implementation**: Claude Code Skill at `.claude/skills/adr/`

**Capabilities**:
- Provides complete ADR template with all required sections
- Guides step-by-step process (numbering, file creation, TOC update)
- Includes best practices and common pitfalls
- Offers example prompts for common ADR types (technology selection, pattern adoption, deprecation, boundaries, tooling)
- References Python script for automated TOC updates

**Supporting Infrastructure**:
- `.claude/skills/adr/scripts/update-adr-toc.py`: Python 3.14+ script for automatic TOC generation
- Scans `docs/adrs/`, extracts metadata, rebuilds table in `docs/adrs/README.md`
- Saves tokens by handling mechanical work without AI assistance

**Rationale for Starting with Skill**:
- Lightweight and easy to iterate
- Provides context and guidance without complex infrastructure
- Tests the ADR workflow before building heavier tooling
- Demonstrates separation of concerns (Skills for guidance, Scripts for automation)

### Phase 2 (Future): Slash Command

**Planned Implementation**: `/adr-new "Decision Title"`

**Capabilities**:
- Quick invocation without manual skill loading
- User-facing workflow shortcut
- Invokes the Skill under the hood

**Triggers**: When skill-based approach proves stable and widely used

### Phase 3 (Future): Sub-Agent for Drafting

**Planned Implementation**: Haiku-based sub-agent for boilerplate generation

**Capabilities**:
- Cost-optimized ADR drafting from conversation context
- Fills template sections based on discussion
- Reduces manual writing while maintaining quality

**Triggers**: When repetitive ADR patterns emerge and justify automation

### Phase 4 (Future): Hooks for Detection

**Planned Implementation**: Git hooks or CI checks

**Capabilities**:
- Detect significant architectural changes in commits
- Prompt for ADR creation if none exists
- Validate ADR format and metadata
- Ensure decisions don't go undocumented

**Triggers**: When ADR adoption is strong and enforcement becomes valuable

### Evolution Principles

1. **Start Simple**: Begin with Skill, add complexity only when needed
2. **Learn and Iterate**: Each phase informs the next
3. **Bottom-Up**: Build on proven patterns, not speculation
4. **No Premature Optimization**: Don't build Phase 4 before Phase 1 proves valuable

### Implementation Details Not Requiring New ADRs

- Moving from Phase 1 to Phase 2/3/4 as planned doesn't require new ADRs
- **Status updates** in this ADR will track phase progression
- **New ADRs required only** if significant architectural changes occur (e.g., abandoning phased approach, choosing different tooling strategy)

## Consequences

### Positive

1. **Token Economics**: Scripts handle mechanical tasks, AI focuses on content and guidance
2. **Consistency**: Template and tooling ensure uniform ADR structure
3. **Lower Barrier**: Guided process encourages ADR adoption
4. **Better Quality**: Examples and best practices improve ADR content
5. **Scalability**: Automated TOC updates scale with growing ADR count
6. **Meta-Learning**: AI agents can reference well-structured ADRs to answer "why" questions
7. **Evolution Path**: Phased approach allows learning and adaptation

### Negative

1. **Additional Tooling**: Scripts and skills require maintenance
2. **Python Dependency**: Contributors need Python 3.14+ for development
3. **Learning Curve**: New contributors must understand the tooling
4. **Overhead**: Even with tooling, ADRs still require thought and writing
5. **Potential Over-Engineering**: Risk of building unused features in later phases

### Neutral

1. **Cultural Shift**: Requires buy-in that ADRs are worth the investment
2. **Evolution Uncertainty**: Later phases may never materialize or may diverge from plan
3. **Reusability**: ADR Skill can be adapted for other OpenXR tutorial projects
4. **Meta-Bootstrapping**: This ADR documents the tooling used to create subsequent ADRs

## References

- [ADR-0000](adr-0000-use-adrs-to-document-architecturally-significant-decisions.md) - Use ADRs to Document Architecturally Significant Decisions
- [ADR-0004](adr-0004-establish-open-ai-code-assistants-structure.md) - Establish Open AI Code Assistants Structure
- [ADR Creation Skill](../../.claude/skills/adr/SKILL.md)
- [ADR TOC Update Script](../../.claude/skills/adr/scripts/update-adr-toc.py)
