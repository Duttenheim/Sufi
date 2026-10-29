#!/usr/bin/env bash
# Generate an Xcode project in ./out. Extra arguments are passed to cmake.
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [[ "$(uname -s)" != "Darwin" ]]; then
    echo "generate.sh creates an Xcode project on macOS. On Windows, run generate.bat." >&2
    exit 1
fi

if ! command -v cmake >/dev/null 2>&1; then
    echo "cmake was not found on PATH." >&2
    exit 1
fi

cmake -S "$root" -B "$root/out" -G Xcode "$@"
