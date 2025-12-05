# ADR-0001: Consolidated Tutorial Repository for OpenXR Quest Samples

**Status**: Accepted

## Context

Developers learning OpenXR development for Meta Quest face several challenges when getting started:

- **Fragmented Learning Materials**: OpenXR samples are scattered across multiple repositories (Khronos OpenXR-SDK-Source, Meta XR samples, community projects)
- **Inconsistent Structure**: Each sample repository uses different directory layouts, build systems, and documentation styles
- **Unclear Dependencies**: Samples don't consistently document SDK requirements, installation steps, or build procedures
- **Onboarding Friction**: New developers spend significant time navigating between repositories, adapting build configurations, and resolving environment setup issues
- **Lack of Progression**: No clear learning path from basic OpenXR concepts to Meta Quest-specific features

The InnoVision development team identified these challenges while onboarding new engineers to Quest 3 native development. Existing samples are valuable but require manual adaptation to work together in a coherent learning environment.

### Alternatives Considered

1. **Link to External Repositories**: Maintain documentation pointing to upstream repositories
   - Pro: Less maintenance burden, always current with upstream
   - Con: No control over structure consistency, broken links over time, no unified learning path

2. **Fork Individual Repositories**: Fork each sample repository independently
   - Pro: Can customize each sample independently
   - Con: Fragmented maintenance, no shared infrastructure, difficult to manage multiple forks

3. **Private Internal Repository**: Keep consolidated samples internal to InnoVision
   - Pro: Full control, can include proprietary modifications
   - Con: Misses opportunity to contribute to community, samples use open-source SDKs anyway

4. **Consolidated Public Repository** (chosen): Single repository with adapted samples
   - Pro: Unified structure, coherent learning path, community contribution, consistent documentation
   - Con: Responsibility to keep samples synchronized with upstream changes

## Decision

We will establish a **public GitHub repository** (`openxr-quest-tutorial`) that consolidates OpenXR and Meta XR native SDK samples into a coherent, well-documented learning resource.

### Repository Characteristics

1. **Tutorial-Focused**: Primary goal is education and onboarding, not production development
2. **Quest-Centric**: Samples prioritize Meta Quest 3 as the target platform
3. **Adapted, Not Forked**: Samples are restructured for consistency while preserving upstream attribution
4. **Self-Contained**: Each sample includes all dependencies needed for learning (may duplicate code across samples)
5. **Public Open-Source**: Licensed for public sharing (samples derive from open-source SDKs)

### Source Material

- **Khronos OpenXR-SDK-Source**: `hello_xr` sample (reference implementation)
- **Meta XR Native Samples**: Quest-specific OpenXR features (passthrough, layer composition, hand tracking)
- **Community Samples**: Additional learning resources as appropriate

### Repository Structure

```
openxr-quest-tutorial/
├── README.md                    # Human-readable overview
├── CLAUDE.md                    # AI agent instructions (repo root)
├── docs/
│   ├── adrs/                    # Architecture Decision Records
│   ├── Environment_Setup.md     # SDK installation guide
│   ├── Build_Deploy_Guide.md    # Build/deploy workflows
│   ├── Repository_Structure.md  # Directory layout
│   └── samples/                 # Sample-specific analysis
├── samples/
│   ├── hello_xr/                # Basic OpenXR sample
│   ├── quest_passthrough/       # Meta passthrough sample
│   └── ...                      # Additional samples
└── .github/
    └── workflows/               # CI/CD for sample validation
```

### Key Design Principles

1. **Documentation-Driven**: AI agent (Claude Code) instructions at root, detailed docs in `docs/`
2. **System-Level SDKs**: OpenXR SDK, Meta XR SDK, Android NDK installed globally (not in repo)
3. **Consistent Build System**: Each sample follows the same CMake + Gradle structure
4. **Progressive Learning**: Samples ordered from basic OpenXR to advanced Quest features
5. **Separate Production Development**: InnoVision SDK and customer applications remain in separate repositories

## Consequences

### Advantages

- **Reduced Onboarding Time**: New developers have a single entry point with consistent structure
- **Clear Learning Path**: Samples progress from basic concepts to advanced features
- **Unified Documentation**: Environment setup, build procedures, and deployment documented once
- **Community Contribution**: Public repository allows others to benefit and contribute
- **Maintenance Clarity**: Single repository to update when SDK versions change
- **AI-Friendly**: Claude Code and other AI agents can navigate the entire tutorial context efficiently

### Disadvantages

- **Maintenance Responsibility**: Must track upstream changes in OpenXR and Meta XR SDKs
- **Synchronization Overhead**: Samples may diverge from upstream if not regularly updated
- **Licensing Complexity**: Must ensure proper attribution for adapted samples
- **Public Commitment**: Repository quality reflects on InnoVision (requires ongoing maintenance)

### Risks & Mitigations

| Risk | Mitigation |
|------|------------|
| Samples become outdated | Document SDK versions explicitly, create CI/CD to test builds |
| Upstream breaking changes | Track upstream releases, document migration in ADRs |
| Licensing conflicts | Verify all samples use compatible open-source licenses (Apache 2.0, MIT) |
| Maintenance burden | Start with 3-5 core samples, expand gradually based on community interest |

### Impact on Production Development

- **No Direct Impact**: Production InnoVision SDK and customer apps remain in separate repositories
- **Indirect Benefit**: Faster onboarding reduces time to productivity for new engineers
- **Knowledge Transfer**: Tutorial patterns inform production code reviews and architecture discussions

### Success Metrics

- New engineer onboarding time reduced from ~2 weeks to ~3 days
- Community engagement: GitHub stars, issues, pull requests
- Sample build success rate: All samples build successfully on documented SDK versions
- Documentation clarity: New engineers can set up environment without additional support

## References

- [OpenXR-SDK-Source Repository](https://github.com/KhronosGroup/OpenXR-SDK-Source)
- [Meta XR Native Samples](https://developer.oculus.com/downloads/native-samples/)
- [Khronos OpenXR Specification](https://www.khronos.org/registry/OpenXR/)
- [Meta Quest Developer Documentation](https://developer.oculus.com/documentation/native/)
- [Project Plan](../Project_Plan.md) - Current project phase and status
- [Repository Structure](../Repository_Structure.md) - Detailed directory layout
