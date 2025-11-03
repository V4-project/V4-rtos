# Changelog - HAL Integration

All notable changes to the V4 RTOS HAL integration will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Initial CMake integration for V4-hal v0.2.0
- Support for local source directory via `V4HAL_LOCAL_PATH` variable
- Support for fetching V4-hal from Git via `FetchContent`
- Platform configuration inheritance via `HAL_PLATFORM` variable
- Alias target `v4_hal` for consistent linking across V4 RTOS components

### Changed
- N/A

### Fixed
- N/A

### Removed
- N/A

## [0.1.0] - 2025-01-03

### Added
- HAL integration directory structure
- README.md with usage documentation

[Unreleased]: https://github.com/V4-project/V4-rtos/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/V4-project/V4-rtos/releases/tag/v0.1.0
