# Architecture Decision Records

This directory contains Architecture Decision Records (ADRs) for the OpenXR Quest Tutorial project.

## What are ADRs?

ADRs document architecturally significant decisions—choices that affect the structure, non-functional characteristics, dependencies, interfaces, or construction techniques of the project.

## Purpose

ADRs serve to:
- Preserve the reasoning behind architectural choices
- Provide a shared understanding of architectural principles
- Help contributors understand decision rationale
- Enable the project to evolve systematically
- Prevent re-examination of previously settled questions

## Format

Each ADR follows this structure:
- **Status**: Proposed, Accepted, Deprecated, Superseded
- **Context**: The issue motivating this decision
- **Decision**: The change being proposed or implemented
- **Consequences**: The resulting context after applying the decision

## Current ADRs

| ADR Number | Title | Status |
|------------|-------|--------|
| [ADR-0000](adr-0000-use-adrs-to-document-architecturally-significant-decisions.md) | Use ADRs to Document Architecturally Significant Decisions | Accepted |
| [ADR-0001](adr-0001-consolidated-tutorial-repository-for-openxr-quest-samples.md) | Consolidated Tutorial Repository for OpenXR Quest Samples | Accepted |
| [ADR-0002](adr-0002-workstation-level-prerequisite-installation.md) | Workstation-Level Prerequisite Installation | Accepted |
| [ADR-0003](adr-0003-use-ovr-openxr-mobile-sdk-for-meta-extensions.md) | Use OVR OpenXR Mobile SDK for Meta Extensions | Accepted |
| [ADR-0004](adr-0004-establish-open-ai-code-assistants-structure.md) | Establish Open AI Code Assistants Structure | Accepted |
| [ADR-0005](adr-0005-openxr-sdk-dependency-management-via-maven-and-fetchcontent.md) | OpenXR SDK Dependency Management via Maven and FetchContent | Accepted |
| [ADR-0006](adr-0006-use-ai-assisted-tooling-for-adr-creation.md) | Use AI-Assisted Tooling for ADR Creation | Accepted |
| [ADR-0007](adr-0007-build-and-run-architecture.md) | Build and Run Architecture | Proposed |
| [ADR-0008](adr-0008-adapt-meta-openxr-samples-using-established-build-blueprint.md) | Adapt Meta OpenXR Samples Using Established Build Blueprint | Proposed |

## Creating New ADRs

ADRs are numbered sequentially (ADR-0000, ADR-0001, etc.) and stored as Markdown files in this directory.

## References

- [Michael Nygard's ADR documentation](https://cognitect.com/blog/2011/11/15/documenting-architecture-decisions)
- [ADR GitHub Organization](https://adr.github.io/)
