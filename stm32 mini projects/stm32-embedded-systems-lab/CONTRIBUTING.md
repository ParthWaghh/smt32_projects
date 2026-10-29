# Contributing to STM32 Embedded Systems Lab

Thank you for your interest in contributing. This repository is a personal learning portfolio, but code reviews, bug reports, and improvement suggestions are welcome.

## How to contribute

1. **Fork** the repository.
2. Create a feature branch: `git checkout -b feature/your-description`.
3. Follow the [coding guidelines](docs/coding-guidelines.md).
4. Ensure your changes compile cleanly with STM32CubeIDE (no warnings at `-Wall`).
5. Update the relevant project README if your change modifies behaviour.
6. Submit a **Pull Request** with a clear description of what changed and why.

## Reporting issues

Open a GitHub Issue with:

- The project number and file(s) affected.
- Steps to reproduce (if applicable).
- Expected vs. actual behaviour.
- Your toolchain version (STM32CubeIDE, compiler, KiCad).

## Code style

See [`docs/coding-guidelines.md`](docs/coding-guidelines.md) for naming conventions, formatting rules, and module structure.

## Scope

Contributions that align with the learning objectives are most useful:

- Bug fixes in firmware source.
- Improvements to test plans or documentation clarity.
- Additional test cases or edge-case coverage.
- KiCad schematic corrections.

Feature additions that introduce new peripherals or projects should be discussed in an Issue first.

## License

By contributing, you agree that your contributions will be licensed under the [MIT License](LICENSE).
