#!/usr/bin/env bash

set -euo pipefail

# ---------------------------------------------------------
# Configuration
# ---------------------------------------------------------

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
DEPENDENCY_DIR="$BUILD_DIR/dependencies"

NTL_PREFIX="/opt/ntl"
INSTALL_PREFIX="/usr/local"

CC="${CC:-gcc-13}"
CXX="${CXX:-g++-13}"

GF2X_VERSION="1.3.0"
NTL_VERSION="11.6.0"

BUILD_TYPE="Release"
PACKAGE_TESTS="ON"
JOBS=4

# ---------------------------------------------------------
# Command-line arguments
# ---------------------------------------------------------

usage() {
    cat <<EOF
Usage: $0 [options]

Options:
    --prefix PATH          NTL/GF2X installation prefix (default: /opt/ntl)
    --install-prefix PATH  data installation prefix (default: /usr/local)
    --no-tests             Disable data tests
    --build-type TYPE      CMake build type (default: Release)
    -h, --help             Show this help

Compiler selection can be overridden using CC and CXX environment variables.
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --prefix)
            NTL_PREFIX="$2"
            shift 2
            ;;
        --install-prefix)
            INSTALL_PREFIX="$2"
            shift 2
            ;;
        --no-tests)
            PACKAGE_TESTS="OFF"
            shift
            ;;
        --build-type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        --jobs=*)
            JOBS="${1#*=}"
            shift
            ;;
        *)
            echo "Unknown option: $1"
            usage
            exit 1
            ;;
    esac
done

# ---------------------------------------------------------
# Helpers
# ---------------------------------------------------------

die() {
    echo "Error: $*" >&2
    exit 1
}

require_command() {
    command -v "$1" >/dev/null 2>&1 ||
        die "Required command not found: $1"
}

download() {
    local url="$1"
    local output="$2"
    local temporary="${output}.part"

    rm -f "$temporary"

    if command -v curl >/dev/null 2>&1; then
        curl --fail --location --show-error --retry 3 \
            --output "$temporary" "$url" || {
                rm -f "$temporary"
                return 1
            }
    elif command -v wget >/dev/null 2>&1; then
        wget --tries=3 --output-document="$temporary" "$url" || {
            rm -f "$temporary"
            return 1
        }
    else
        die "Either curl or wget is required."
    fi

    if ! gzip -t "$temporary" 2>/dev/null; then
        echo "Downloaded file is not a valid gzip archive: $url"
        rm -f "$temporary"
        return 1
    fi

    mv "$temporary" "$output"
}

install_prefix() {
    if [[ "$(id -u)" -eq 0 ]] || [[ -w "$1" ]]; then
        "$@"
    else
        sudo "$@"
    fi
}

# ---------------------------------------------------------
# Check dependencies
# ---------------------------------------------------------

echo
echo "Checking dependencies..."

for command in "$CC" "$CXX" cmake ninja make tar autoreconf; do
    require_command "$command"
done

echo "Compiler: $("$CXX" --version | head -n 1)"
echo "NTL prefix: $NTL_PREFIX"
echo "Install prefix: $INSTALL_PREFIX"
echo "Tests: $PACKAGE_TESTS"

# ---------------------------------------------------------
# Prepare directories
# ---------------------------------------------------------

mkdir -p "$DEPENDENCY_DIR"

# Ensure the installation prefix exists.
if [[ ! -d "$NTL_PREFIX" ]]; then
    install_prefix mkdir -p "$NTL_PREFIX"
fi

cd "$DEPENDENCY_DIR"

# ---------------------------------------------------------
# Build GF2X
# ---------------------------------------------------------

echo
echo "========================================"
echo "Building GF2X $GF2X_VERSION"
echo "========================================"

GF2X_ARCHIVE="gf2x-${GF2X_VERSION}.tar.gz"
GF2X_SOURCE="gf2x-${GF2X_VERSION}"

GF2X_URL="https://gitlab.inria.fr/gf2x/gf2x/-/archive/gf2x-${GF2X_VERSION}/gf2x-gf2x-${GF2X_VERSION}.tar.gz"
GF2X_FALLBACK_URL="https://deb.debian.org/debian/pool/main/g/gf2x/gf2x_${GF2X_VERSION}.orig.tar.gz"

if [[ ! -f "$GF2X_ARCHIVE" ]] || ! gzip -t "$GF2X_ARCHIVE" 2>/dev/null; then
    rm -f "$GF2X_ARCHIVE"

    echo "Downloading GF2X from GitLab..."
    if ! download "$GF2X_URL" "$GF2X_ARCHIVE"; then
        rm -f "$GF2X_ARCHIVE"

        echo "GitLab download failed. Trying Debian mirror..."
        download "$GF2X_FALLBACK_URL" "$GF2X_ARCHIVE"
    fi
fi

if [[ ! -d "$GF2X_SOURCE" ]]; then
    mkdir -p "$GF2X_SOURCE"
    tar -xzf "$GF2X_ARCHIVE" \
        --strip-components=1 \
        -C "$GF2X_SOURCE"
fi

cd "$GF2X_SOURCE"

# ---------------------------------------------------------
# Build NTL
# ---------------------------------------------------------

echo
echo "========================================"
echo "Building NTL $NTL_VERSION"
echo "========================================"

NTL_ARCHIVE="ntl-${NTL_VERSION}.tar.gz"
NTL_URL="https://libntl.org/${NTL_ARCHIVE}"
NTL_SOURCE="ntl-${NTL_VERSION}"

if [[ ! -f "$NTL_ARCHIVE" ]]; then
    download "$NTL_URL" "$NTL_ARCHIVE"
fi

if [[ ! -d "$NTL_SOURCE" ]]; then
    tar -xf "$NTL_ARCHIVE"
fi

cd "$NTL_SOURCE/src"

./configure \
    CXX="$CXX" \
    PREFIX="$NTL_PREFIX" \
    GMP_PREFIX=/usr \
    GF2X_PREFIX="$NTL_PREFIX" \
    NTL_GF2X_LIB=on \
    SHARED=off

make -j"$JOBS"
make check

install_prefix make install

# ---------------------------------------------------------
# Configure data
# ---------------------------------------------------------

echo
echo "========================================"
echo "Configuring data"
echo "========================================"

cd "$SCRIPT_DIR"

cmake -S . -B "$BUILD_DIR" -G Ninja \
    -U NTL_INCLUDE_DIR \
    -U NTL_LIBRARY \
    -U GF2X_LIBRARY \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" \
    -DNTL_PREFIX="$NTL_PREFIX" \
    -DPACKAGE_TESTS="$PACKAGE_TESTS" \
    -DCMAKE_COLOR_DIAGNOSTICS=ON \
    -DCMAKE_CXX_FLAGS=-fdiagnostics-color=always

# ---------------------------------------------------------
# Build data
# ---------------------------------------------------------

echo
echo "========================================"
echo "Building data"
echo "========================================"

cmake --build "$BUILD_DIR" -j"$JOBS"

# ---------------------------------------------------------
# Test data
# ---------------------------------------------------------

if [[ "$PACKAGE_TESTS" == "ON" ]]; then
    echo
    echo "========================================"
    echo "Testing data"
    echo "========================================"

    export LD_LIBRARY_PATH="$NTL_PREFIX/lib:$NTL_PREFIX/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

    ctest --test-dir "$BUILD_DIR" --output-on-failure
fi

# ---------------------------------------------------------
# Install data
# ---------------------------------------------------------

echo
echo "========================================"
echo "Installing data"
echo "========================================"

if [[ "$(id -u)" -eq 0 ]] || [[ -w "$INSTALL_PREFIX" ]]; then
    cmake --install "$BUILD_DIR"
else
    sudo cmake --install "$BUILD_DIR"
fi

echo
echo "========================================"
echo "Build completed successfully!"
echo "========================================"

echo "NTL/GF2X installed under: $NTL_PREFIX"
echo "data installed under:     $INSTALL_PREFIX"
