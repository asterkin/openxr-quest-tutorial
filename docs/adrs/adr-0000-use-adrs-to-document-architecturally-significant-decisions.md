# ADR-0000: Use ADRs to Document Architecturally Significant Decisions

**Status**: Accepted

## Context

Architectural decisions accumulate throughout a project's lifecycle, but without documentation, teams face several challenges:

- **Invisible Constraints**: Undocumented design choices create hidden constraints that limit future options
- **Difficult Onboarding**: New contributors struggle to understand the rationale behind existing design decisions
- **Fading Knowledge**: As team members change, institutional knowledge about why certain decisions were made is lost
- **Wasted Effort**: Teams re-examine previously settled questions, duplicating past decision-making work
- **Limited AI Context**: Automated systems and AI assistants lack insight into decision rationale without explicit documentation

The OpenXR Quest Tutorial project needs a systematic way to capture architectural decisions that balances thoroughness with practicality.

## Decision

We will use Architecture Decision Records (ADRs) as the standard mechanism for documenting architecturally significant decisions.

### Storage & Format

- **Location**: `docs/adrs/` directory under version control
- **Format**: Markdown files with sequential numbering (ADR-0000, ADR-0001, etc.)
- **Structure**: Each ADR includes Status, Context, Decision, Consequences, and References sections

### Scope Definition

Architecturally significant decisions include choices that:

1. **Influence system structure**: Changes to the fundamental organization of the codebase or project
2. **Affect quality characteristics**: Impact performance, maintainability, portability, or other quality attributes
3. **Define external dependencies**: Selection of SDKs, libraries, build tools, or target platforms
4. **Create long-term implications**: Decisions that are costly or difficult to reverse
5. **Balance competing priorities**: Trade-offs between different technical objectives
6. **Set patterns or standards**: Establish conventions followed throughout the project
7. **Address technical uncertainties**: Resolve previously debated technical questions

### What NOT to Document

- Implementation details that don't affect architecture
- Temporary workarounds or quick fixes
- Routine bug fixes or minor refactoring
- Decisions easily reversed without significant impact

## Consequences

### Advantages

- **Enhanced Transparency**: Team members understand the reasoning behind architectural choices
- **Streamlined Onboarding**: New contributors can quickly grasp design rationale
- **Prevention of Duplicate Work**: Avoid re-examining previously settled questions
- **Stronger Team Alignment**: Shared understanding of architectural principles
- **AI Accessibility**: AI assistants (like Claude Code) can access decision context
- **Preserved Learning**: Organizational knowledge survives team changes

### Disadvantages

- **Documentation Discipline Required**: Team must maintain consistency in documenting decisions
- **Ongoing Maintenance**: ADRs may need updates as decisions evolve or are superseded
- **Initial Delays**: May slow down decision-making initially as team adapts to the practice

### Considerations

- ADRs are documentation tools, not decision guarantees
- Success depends on team adoption and commitment
- Should be lightweight enough to encourage use, detailed enough to be valuable

## References

- [Documenting Architecture Decisions - Michael Nygard](https://cognitect.com/blog/2011/11/15/documenting-architecture-decisions)
- [ADR GitHub Organization](https://adr.github.io/)
- [Markdown Architectural Decision Records](https://adr.github.io/madr/)
