# SST-Claude: Semantic Spacetime Model for Claude Tooling Optimization

## Project Vision: Agent Development Kit for Meta Quest OpenXR

**Objective:** An "Agent VM program" that provides stepwise guidance for building Quest 3 native applications using NDK + OpenXR.

### Bootstrap

Single installation script (Claude Code CLI + ADK plugin) — the **only manual prerequisite**.

### Agent-Guided Workflow (FOLLOWS chain)

1. Environment setup (SDK, NDK, device verification)
2. Skeleton project generation
3. Input handler integration
4. Rendering engine setup
5. Camera/passthrough configuration
6. *[additional stages TBD]*

### Principle

Minimize manual steps to ONE, agent handles the rest.

### Raw Materials → Output

- **Input:** Official Meta samples, OpenXR tutorials (this repository's original content)
- **Output:** Working application + developer understanding at each stage

### Value Proposition Canvas (future refinement)

| Customer Jobs | Pains | Gains |
|---------------|-------|-------|
| Build Quest 3 app without deep XR expertise | Complex toolchain, sparse docs, trial-and-error debugging | Working code at each step, contextual guidance |

*This vision statement will evolve into a complete VPC specification.*

---

## Declaration of Intents

This document summarizes research direction for optimizing Claude tool interactions using Semantic Spacetime (SST) conceptual model.

## Problem Statement

**Goal:** Maximize accuracy while minimizing token consumption (mini-max optimization)

- **Accuracy metric:** Inverse of retry count (automatic or user-feedback driven)
- **Cost metric:** Tokens spent per successful outcome
- **Constraint:** Pro subscription rate limits (session and weekly)

Current pain points observed in Android C++/Quest development:
- Reading long (1000+ lines) C++ source files
- Processing verbose Quest-3 logs
- Parsing long Gradle build errors

## Core Hypothesis

Claude's information space can be modeled as a traversable graph where:
- **Context window** = previously visited nodes
- **Possibilities** = nodes accessible from current position
- **Optimization** = finding minimal-cost paths to goal nodes

Using SST's four connection types as universal vocabulary improves signal-to-noise ratio by letting Claude work with familiar, minimal semantics rather than domain-specific noise.

## SST Connection Types

| Type | Meaning | Example |
|------|---------|---------|
| CONTAINS / CONTAINED-IN | Hierarchical structure | Package contains Module |
| FOLLOWS / FOLLOWED-BY | Temporal/dependency flow | A imports B → A follows B |
| EXPRESSES / EXPRESSED-BY | Description, metadata | Module described-by pydoc |
| NEAR | Proximity (often signals inefficiency) | Co-accessed nodes lacking explicit link |

**Note on NEAR:** In well-structured graphs, persistent "near" patterns indicate refactoring opportunities—nodes frequently accessed together should be consolidated or explicitly linked.

## Token Cost Hierarchy

| Layer | Media Type | Load Pattern | Token Cost |
|-------|-----------|--------------|------------|
| 0 | ~/CLAUDE.md | Always full read | Fixed baseline |
| 0.5 | Rules (.claude/rules/*.md) | Auto-loaded, path-conditional | Variable (use `paths:` frontmatter to limit) |
| 1 | Skills YAML prefixes | Conditional, prefix only | Low |
| 2 | Parsed structures (AST, sections) | On-demand fragments | Variable, optimized |
| 3 | Backend servers (GitHub, etc.) | Query-response | Similar to Layer 2 |

**Key principle:** CLAUDE.md serves as routing table defining how to traverse into deeper layers economically, not as content container.

**Rules optimization:** Use `paths:` frontmatter in rules to limit when they load. A rule with `paths: src/api/**/*.ts` only loads when working on API files, saving tokens when working elsewhere.

## Graph Representation Format

Rejected approaches:
- RDF/Turtle triples: Token-inefficient (subject repeated per edge)
- Mermaid: Visualization-first, not traversal-optimized

Preferred: **Node-centric YAML** (adjacency list with typed edges)

```yaml
LibraryL1:
  described-by: README.md#overview
  follows: Library2  # depends on
  contains: PackageA, PackageB

PackageA:
  contained-in: Library1
  described-by: __init__.py  # __version__, __all__, etc.
  contains: ModuleX, PackageB

ModuleX:
  contained-in: PackageA
  contains:
    ClassC1:
      described-by: pydoc
      contains:
        methodM1:
          described-by: pydoc
          follows: Library2.PackageP.functionF1
```

**Critical insight:** One hop = one retrieval unit. Graph structure aligns with access boundaries.

## Hierarchical CLAUDE.md Architecture

Each level refines semantic scope:

1. **Top-level ~/CLAUDE.md:** General access patterns, script-based retrieval rules
2. **Project CLAUDE.md:** Domain rules (mono-repo structure, package relationships)
3. **Language-specific CLAUDE.md:** Code graph semantics (AST patterns, import rules)

Example optimization instruction:
> "Use `get_section.sh <file> <section>` to retrieve specific Markdown sections rather than reading entire files."

## Static vs. Dynamic Graphs

- **Static:** Project structure (files, AST, schemas)—what Claude typically operates on
- **Dynamic:** Session traversal, execution traces, temporal flow

The model has two layers:
- Static graph informs *what's reachable*
- Dynamic graph informs *what's been visited* and *what typically follows*

## Implementation Strategy

**Approach:** Bottom-up, experience-driven

```
quest-openxr-tutorial     →  InnoVision customer work
        ↓                            ↓
   SST patterns emerge    →    patterns validated/refined
        ↓                            ↓
             commonality analysis
                    ↓
              sst-claude (when earned)
```

**Rationale:**
- Avoid premature abstraction (rule of three)
- Avoid gold-rush bloat (too much, too early)
- Patterns must prove themselves before generalization
- Domain-specific adapters only when actually needed

## Potential Application Areas

1. C++/CMake/Gradle build systems (current pain point)
2. Log analysis and error extraction
3. ADRs and project documentation
4. GitHub/GitFlow interaction
5. User navigation state machines
6. Value proposition → features → implementation progression

## Key Finding: Two Complementary SST Graphs

Claude Code's inner structure can be modeled by two complementary SST graphs:

### Graph 1: Claude Code Elements

```yaml
claude-code:
  follows: .claude/, CLAUDE.md

.claude/:
  contains:
    - settings.json, settings.local.json  # permissions
    - rules/     # *.md context-triggered headers (auto-loaded)
    - skills/    # SKILL.md per skill (on-demand invocation)
    - agents/    # sub-agents
    - commands/  # slash commands
    - servers/   # MCP services

CLAUDE.md:
  contains: # implicitly derived from Markdown headings
```

### Graph 2: Model/Depth Lattice

```
         FOLLOWS (escalate) →
         ← FOLLOWS (delegate)

         ┌─────────┬─────────┬─────────┐
   Ultra │ H-ultra │ S-ultra │ O-ultra │
         ├─────────┼─────────┼─────────┤
   Heavy │ H-heavy │ S-heavy │ O-heavy │
         ├─────────┼─────────┼─────────┤
    Lite │ H-lite  │ S-lite  │ O-lite  │
         └─────────┴─────────┴─────────┘
           Haiku    Sonnet    Opus

         ↑↓ FOLLOWS (depth change)
```

All transitions are FOLLOWS relationships. Direction determines meaning:
- **Escalation**: Haiku → Sonnet → Opus (problem cannot be handled locally)
- **Delegation**: Opus → Sonnet → Haiku (push execution to cheaper model)
- **Depth change**: Lite ↔ Heavy ↔ Ultra (adjust reasoning intensity)

### Kahneman-Inspired Flow (Thinking Fast/Slow)

- **Problems flow UP**: Haiku attempts first; if pattern unrecognized, escalates
- **Knowledge flows DOWN**: Opus discovers solutions, codifies them for lower models

### Transition Cost Model (Game Theory Extension)

Each transition has associated:
- **Cost**: Token/compute expense of the transition
- **Penalty**: Risk of incorrect escalation/delegation

Statechart rules (TBD via research or self-learning):
- **Strict mode**: Must traverse intermediate nodes
- **Emergency mode**: Direct jumps allowed (e.g., H-lite → O-ultra)
- **Optimized mode**: Learned shortcuts based on observed outcomes

---

*Added from Claude Code CLI research conversation, January 2025*

---

## Open Questions

1. **Overhead crossover:** At what project size does lazy-loading infrastructure pay off?
2. **Claude compliance:** Will Claude reliably use indirection scripts vs. reading files directly?
3. **FOLLOWS ambiguity:** Dependency vs. temporal ordering—domain-specific naming may be needed
4. **Token accounting:** Need visibility into actual consumption for validation

## Next Steps

1. Apply SST model to quest-openxr-tutorial as controlled experiment
2. Document learnings in `docs/claude-sst.md`
3. Push proven solutions to customer projects
4. Extract sst-claude generic framework when patterns stabilize

---

*Generated from Claude Desktop research conversation, December 2024*

---

## Claude Code CLI as Non-Deterministic Virtual Machine

An alternative conceptual framework: Claude Code CLI as a **non-deterministic VM** with approximate mappings to traditional computing concepts.

### Strong Analogies

| Claude Code Concept | VM Equivalent | Notes |
|---------------------|---------------|-------|
| CLAUDE.md | `main()` entry point | Establishes execution context, imports parent configs |
| Rules (.claude/rules/*.md) | Header files / includes | Auto-loaded, path-conditional via frontmatter `paths:` glob |
| Skills | Runtime library | Pre-loaded, always available, invoked by name |
| Context window | Limited RAM | Hard ceiling, no virtual memory equivalent |
| Context compaction | Lossy GC / cache eviction | Unlike GC, information is permanently lost |
| Sub-agents | Subprocesses | Isolated memory, own config, IPC via return values |
| Scripts (Python, etc.) | FFI (JNI, ctypes, Rust in Python) | Deterministic extensions callable from non-deterministic host |
| Plugins | RPM/DEB packages | Installable bundles containing skills, hooks, MCP configs—mixed element types |

### Weaker Analogies

| Claude Code Concept | VM Equivalent | Why It Differs |
|---------------------|---------------|----------------|
| User prompts/responses | stdin/stdout pipes | Bidirectional within single call; stateful; semantic, not byte-stream |
| `/clear` command | Process restart | More like `fork()` + `exec()` without fork—no state preserved anywhere |
| Hooks | Event handlers / OS signals | External shell commands, not internal handlers |
| MCP servers | External services / RPC | Can be expensive context-wise; looser coupling than typical IPC |

### Novel Observations

**Configuration as Programming**
- Traditional: deterministic instructions → predictable output
- Agent programming: probabilistic constraints → bounded stochastic behavior
- Exact Agent Engineering practices are yet to be discovered

**SST as RISC Model**
- Initial hypothesis: SST reduces operations to semantic primitives
- Counter-observation: SST describes *emergent patterns* from complex base, not building up from primitives
- **Resolution:** See [SST as Stochastic RISC](#sst-as-stochastic-risc) section below

**Dijkstra Layering Applicability**
- Strict layering assumes deterministic contracts between layers
- Non-determinism breaks these guarantees
- Alternative models to explore: actor semantics, probabilistic contracts

### Missing from Traditional VM Analogy

| Concept | Why No Equivalent Exists |
|---------|--------------------------|
| Temperature/sampling | RNG seed is closest, but fundamentally different—affects reasoning, not just randomness |
| Attention mechanism | No spatial locality; context is "flat" unlike hierarchical memory |
| Model weights | "Instruction set" is learned, not designed—CPU that evolved rather than was engineered |

### SST as Stochastic RISC

The RISC analogy is legitimate. SST defines a **probabilistic instruction set architecture** for agent execution.

#### Primitive Operations (8 ops from 4 connection types)

| Operation | Connection Type | Description |
|-----------|-----------------|-------------|
| `follow` | FOLLOWS | Proceed through FOLLOWS link |
| `return` | FOLLOWED_BY | Proceed through reverse link |
| `zoom-in` | CONTAINS | Enter container (entry points detected via FOLLOWS analysis) |
| `zoom-out` | CONTAINED_BY | Exit container |
| `read` | REPRESENTED_BY | Retrieve element's representation |
| `search` | REPRESENTS | Find all representations of element |
| `compare` | (derived) | Measure distance between elements via representations |
| `next` / `prev` | NEAR (ordered) | Follow ordered proximity links |

These operations are **orthogonal, composable, and complete** for graph traversal—mirroring RISC's "small set of orthogonal instructions."

#### Execution Model: Promise Theory

Transition firing is regulated by Promise Theory:
- **Promise to accept** — receiver's willingness to process
- **Promise to deliver** — sender's commitment to provide

Minimal overlap level functions analogously to temperature—a configuration parameter controlling transition probability.

#### Implementation Independence

The instruction set doesn't prescribe execution strategy:

| Strategy | Description | Trade-offs |
|----------|-------------|------------|
| Top-down (Opus → Haiku) | Start expensive, delegate down | High accuracy, higher cost |
| Bottom-up (Haiku → Opus) | Start cheap, escalate up | Lower cost, potential retries |
| Adaptive | Learn optimal routing | Development complexity, best long-term |

Same core functionality, different non-functional characteristics (cost, accuracy, latency, complexity).

#### Resolution of the RISC Paradox

The original counter-observation confused *discovery* with *operation*:
- SST was **discovered** by observing emergent patterns (top-down analysis)
- SST **operates** as composable primitives (bottom-up execution)

These aren't contradictory. The 8 operations are opcodes; Promise Theory is the execution model.

**Key differentiator from traditional RISC:** Operations are probabilistic, not deterministic. This extends rather than breaks the analogy—yielding **Stochastic RISC**.

---

*Added from SST-RISC analysis discussion, January 2026*

---

### Token Economics

| Aspect | Cloud Computing Equivalent |
|--------|---------------------------|
| 5-hour window budget | Serverless compute limits |
| Weekly token allocation | Reserved capacity |
| Budget exceeded → Pay-per-use | Burst pricing / on-demand instances |
| Subscription tiers | VM size tiers (t2.micro → m5.xlarge) |

Key difference: measured in **tokens** (semantic units) rather than **time** (compute units).

### Implications for Agent Engineering

1. **Determinism boundary**: Scripts provide deterministic islands within stochastic execution
2. **Memory management**: Unlike VMs, "forgetting" is an inherent property, not a bug
3. **Cost optimization**: Minimize context usage while maximizing task completion (mini-max)
4. **Composability**: Sub-agents as isolation boundaries for complex workflows

---

*Added from Claude Code CLI VM analogy discussion, January 2026*
