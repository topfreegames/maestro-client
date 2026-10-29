#!/usr/bin/env bash
# Runs inside maestro-builder-arm64. See run.sh.
set -uo pipefail

BUILD_DIR=_builds/stress-arm64
CXX_FLAGS="-Wno-error -Wno-deprecated-declarations"
LINKER_FLAGS="-Wl,--no-as-needed -lcurl -Wl,--as-needed"
if [[ -n "${SANITIZE:-}" ]]; then
  CXX_FLAGS="$CXX_FLAGS -g -fno-omit-frame-pointer -fsanitize=$SANITIZE"
  LINKER_FLAGS="$LINKER_FLAGS -fsanitize=$SANITIZE"
  BUILD_DIR="$BUILD_DIR-$SANITIZE"
  # Client never frees its connection; that leak is known and not what we test.
  export ASAN_OPTIONS=detect_leaks=0
fi

if ! command -v python3 >/dev/null; then
  apt-get update -q >/dev/null && apt-get install -yq python3 >/dev/null
fi

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
BUILD_LOG="$BUILD_DIR/build.log"
if ! { cmake -H. -B"$BUILD_DIR" -GNinja -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_PREFIX_PATH=/opt/restclient-arm64 -DBOOST_FIND_POLICY=OLD \
         -DBUILD_SHARED_LIBS=ON -DCMAKE_CXX_STANDARD=14 -DCMAKE_CXX_STANDARD_REQUIRED=ON \
         -DCMAKE_CXX_FLAGS="$CXX_FLAGS" \
         -DCMAKE_SHARED_LINKER_FLAGS="$LINKER_FLAGS" -DCMAKE_EXE_LINKER_FLAGS="$LINKER_FLAGS" \
         -DMAESTRO_BUILD_STRESS=ON \
       && cmake --build "$BUILD_DIR" --target stress; } >"$BUILD_LOG" 2>&1; then
  cat "$BUILD_LOG"
  echo "FAIL: build failed"
  exit 1
fi

python3 tests/concurrency/flaky_server.py 5000 &
SERVER_PID=$!
trap 'kill $SERVER_PID 2>/dev/null' EXIT
sleep 1

ulimit -c 0
echo "running stress for ${DURATION}s against the flaky server..."
SECONDS=0
# The client logs every failed call to stdout, so drop it and keep stderr.
"$BUILD_DIR/bin/stress" "$DURATION" >/dev/null
STATUS=$?

if [[ $STATUS -eq 0 ]]; then
  echo "PASS"
else
  echo "FAIL: stress exited with status $STATUS after ${SECONDS}s"
fi
exit $STATUS
