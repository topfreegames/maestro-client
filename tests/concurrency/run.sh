#!/usr/bin/env bash
# Reproduces SUP-8845 (heap corruption inside libcurl when one Maestro::Client
# is used from several threads) on linux/arm64, using the same builder image
# as the release build.
#
#   tests/concurrency/run.sh                    # 60s run
#   DURATION=120 tests/concurrency/run.sh
#   SANITIZE=address tests/concurrency/run.sh   # AddressSanitizer build
set -euo pipefail

cd "$(dirname "$0")/../.."

docker buildx build -q --platform linux/arm64 -f Dockerfile-builder-arm64 \
  -t maestro-builder-arm64:latest --load . >/dev/null

docker run --rm --platform linux/arm64 -v "$(pwd)":/app-src \
  -e DURATION="${DURATION:-60}" -e SANITIZE="${SANITIZE:-}" \
  maestro-builder-arm64:latest tests/concurrency/run-in-container.sh
