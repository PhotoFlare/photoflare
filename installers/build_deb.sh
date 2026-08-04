#!/usr/bin/env bash
# build_deb.sh
# Builds a PhotoFlare .deb package locally, intended to be run from Ubuntu
# (including Ubuntu-on-WSL).
#
# Usage:
#   bash installers/build_deb.sh [--prefix /path/to/qt6] [--with-gmic]
#
# Options:
#   --prefix PATH   Path to a Qt6 installation (sets QMAKE)
#   --with-gmic     Also clone, build, and bundle G'MIC-Qt as gmic_photoflare_qt
#                   next to the photoflare binary (same as build_appimage.sh).
#                   Requires Qt >= 6.5 and cmake/git/wget/libfftw3-dev/
#                   libcurl4-openssl-dev. Off by default — Ubuntu 24.04's
#                   apt Qt6 packages are 6.4.x, which is below gmic-qt's
#                   minimum supported Qt6 version.
#
# Output: photoflare_<VERSION>~<CODENAME>1_<ARCH>.deb in the repo root, where
# CODENAME is auto-detected from /etc/os-release (e.g. noble on 24.04) and
# runtime Depends are resolved from the build machine's actual libraries, so
# the same command produces a correct package on 24.04, 26.04, etc.
#
# Requirements (Ubuntu):
#   sudo apt-get install -y build-essential dpkg-dev \
#     libgraphicsmagick++-dev libomp-dev \
#     qt6-base-dev qt6-tools-dev qt6-tools-dev-tools qt6-image-formats-plugins
#
# Note on WSL: staging is done under /tmp rather than the repo (which may be
# on an NTFS-backed /mnt/c mount) because chmod is unreliable there and .deb
# packaging requires correct permission bits.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

QT_PREFIX=""
WITH_GMIC=0

while [[ $# -gt 0 ]]; do
    case "$1" in
        --prefix)     QT_PREFIX="$2"; shift 2 ;;
        --with-gmic)  WITH_GMIC=1; shift ;;
        *) echo "Unknown argument: $1"; exit 1 ;;
    esac
done

# Resolve qmake
if [[ -n "$QT_PREFIX" ]]; then
    QMAKE="$QT_PREFIX/bin/qmake"
else
    QMAKE=$(command -v qmake6 || command -v qmake || true)
fi
if [[ -z "$QMAKE" || ! -x "$QMAKE" ]]; then
    echo "ERROR: qmake not found. Install qt6-base-dev or pass --prefix /path/to/qt6"
    exit 1
fi
echo "Using qmake: $QMAKE"

# Install build dependencies
echo "=== Installing build dependencies ==="
if command -v apt-get &>/dev/null; then
    SUDO=""
    [[ $EUID -ne 0 ]] && command -v sudo &>/dev/null && SUDO="sudo"
    $SUDO apt-get install -y \
        build-essential dpkg-dev \
        libgraphicsmagick++-dev libomp-dev \
        qt6-base-dev qt6-tools-dev qt6-tools-dev-tools qt6-image-formats-plugins
    if [[ $WITH_GMIC -eq 1 ]]; then
        $SUDO apt-get install -y cmake git wget libfftw3-dev libcurl4-openssl-dev
    fi
fi
echo ""

# Base version comes from the control file; the distro suffix is derived from
# whatever Ubuntu release this script is actually run on, so the same command
# produces a correctly-labelled package on 24.04, 26.04, etc.
BASE_VERSION="$(grep -oP '(?<=^Version: )\S+' installers/deb/DEBIAN/control)"
if [[ -z "$BASE_VERSION" ]]; then
    echo "ERROR: could not parse Version from installers/deb/DEBIAN/control"
    exit 1
fi
CODENAME="$(. /etc/os-release && echo "${VERSION_CODENAME:-unknown}")"
VERSION="${BASE_VERSION}~${CODENAME}1"
ARCH="$(dpkg --print-architecture)"
DEB_NAME="photoflare_${VERSION}_${ARCH}.deb"

echo "=== Building photoflare ${VERSION} (${ARCH}) ==="

# Use /tmp for build/staging so chmod works reliably on WSL (NTFS mounts ignore chmod)
BUILD_DIR="/tmp/photoflare-deb-build"
STAGING_DIR="${BUILD_DIR}/staging"
rm -rf "$BUILD_DIR"
mkdir -p "$STAGING_DIR"

"$QMAKE" CONFIG+=release CONFIG+=packaging PREFIX=/usr
make -j"$(nproc)"
make install INSTALL_ROOT="$STAGING_DIR"
echo ""

# Resolve the photoflare binary's actual shared-library dependencies to the
# packages that provide them on this build machine, rather than hardcoding
# version-specific names (e.g. Ubuntu 24.04's libXXXt64 renames) that break
# on other releases.
echo "=== Resolving runtime dependencies ==="
# Query one library at a time (not via xargs batching) and tolerate lookup
# failures — with set -e/pipefail, one unowned library (e.g. not installed
# via apt) would otherwise abort the whole pipeline.
SHLIB_DEPENDS="$( (ldd "$STAGING_DIR/usr/bin/photoflare" \
    | awk '{print $3}' | grep '^/' | sort -u \
    | while read -r lib; do dpkg -S "$lib" 2>/dev/null || true; done) \
    | cut -d: -f1 | sort -u | paste -sd, - || true)"
echo "Detected: $SHLIB_DEPENDS"
echo ""

if [[ $WITH_GMIC -eq 1 ]]; then

# Build gmic-qt and bundle it as gmic_photoflare_qt next to the photoflare binary
echo "=== Building gmic-qt ==="
GMIC_QT_CLONE="$REPO_ROOT/build-gmic-qt-src"
GMIC_QT_BUILD="$REPO_ROOT/build-gmic-qt"

CMAKE_PREFIX=""
[[ -n "$QT_PREFIX" ]] && CMAKE_PREFIX="-DCMAKE_PREFIX_PATH=$QT_PREFIX"

if [[ ! -d "$GMIC_QT_CLONE" ]]; then
    git clone --depth 1 --branch v.3.4.2 https://github.com/c-koi/gmic-qt.git "$GMIC_QT_CLONE"
    git clone --depth 1 --branch v.3.7.5 https://github.com/GreycLab/gmic.git "$GMIC_QT_CLONE/gmic"
fi

# Pre-fetch CImg.h from the matching versioned tag — the gmic Makefile's CImg.h
# target downloads from master (latest) which may not match the pinned gmic version.
GMIC_SRC="$GMIC_QT_CLONE/gmic/src"
GMIC_VER=$(grep -oP '(?<=gmic_version )\d+' "$GMIC_SRC/gmic.h")
CIMG_TAG="v.${GMIC_VER:0:1}.${GMIC_VER:1:1}.${GMIC_VER:2:1}"
if [[ ! -s "$GMIC_SRC/CImg.h" ]]; then
    echo "  Downloading CImg.h at tag $CIMG_TAG ..."
    wget --no-check-certificate --quiet -O "$GMIC_SRC/CImg.h" \
        "https://github.com/GreycLab/CImg/raw/${CIMG_TAG}/CImg.h"
fi
if [[ ! -s "$GMIC_SRC/gmic_stdlib_community.h" ]]; then
    echo "  Downloading gmic_stdlib_community.h ..."
    GMIC_VERSION="${CIMG_TAG#v.}"
    wget --quiet -O /tmp/gmic-src.tar.gz \
        "https://github.com/GreycLab/gmic/releases/download/v.${GMIC_VERSION}/gmic_${GMIC_VERSION}.tar.gz" \
        || { echo "Failed to download gmic source tarball"; exit 1; }
    tar -xzf /tmp/gmic-src.tar.gz -C "$GMIC_SRC" --wildcards --strip-components=2 \
        "*/src/gmic_stdlib_community.h" \
        || { echo "Failed to extract gmic_stdlib_community.h from tarball"; exit 1; }
    rm -f /tmp/gmic-src.tar.gz
fi

# Ensure lrelease (Qt6) is on PATH for the gmic-qt translations Makefile
if ! command -v lrelease &>/dev/null && ! command -v lrelease-qt5 &>/dev/null; then
    QT6_LRELEASE=$(find /usr/lib/qt6/bin /usr/lib/x86_64-linux-gnu/qt6/bin \
                        /opt/qt6/bin /usr/local/lib/qt6/bin \
                        -name "lrelease" 2>/dev/null | head -1 || true)
    [[ -n "$QT6_LRELEASE" ]] && export PATH="$(dirname "$QT6_LRELEASE"):$PATH"
fi

cmake "$GMIC_QT_CLONE" -B "$GMIC_QT_BUILD" \
    $CMAKE_PREFIX \
    -DBUILD_WITH_QT6=ON \
    -DGMIC_QT_HOST=none \
    -DENABLE_SYSTEM_GMIC=OFF \
    -DGMIC_PATH="$GMIC_QT_CLONE/gmic/src" \
    -DENABLE_FFTW3=ON \
    -DENABLE_CURL=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_EXE_LINKER_FLAGS="-no-pie"

cmake --build "$GMIC_QT_BUILD" --parallel 1
cp "$GMIC_QT_BUILD/gmic_qt" "$STAGING_DIR/usr/bin/gmic_photoflare_qt"
chmod 755 "$STAGING_DIR/usr/bin/gmic_photoflare_qt"
echo ""

fi # WITH_GMIC

# Stage DEBIAN control metadata, refreshing Version, Installed-Size and Depends
echo "=== Staging DEBIAN control ==="
mkdir -p "$STAGING_DIR/DEBIAN"
INSTALLED_SIZE="$(du -sk "$STAGING_DIR/usr" | cut -f1)"
awk -v version="$VERSION" -v size="$INSTALLED_SIZE" -v deps="$SHLIB_DEPENDS" \
    '{
        if ($0 ~ /^Version:/) print "Version: " version;
        else if ($0 ~ /^Installed-Size:/) print "Installed-Size: " size;
        else if ($0 ~ /^Depends:/) print $0 ", " deps;
        else print
    }' \
    installers/deb/DEBIAN/control > "$STAGING_DIR/DEBIAN/control"

# Set correct permissions
find "$STAGING_DIR" -type d -exec chmod 755 {} \;
find "$STAGING_DIR" -type f -exec chmod 644 {} \;
chmod 755 "$STAGING_DIR/DEBIAN"
chmod 755 "$STAGING_DIR/usr/bin/photoflare"
[[ -f "$STAGING_DIR/usr/bin/gmic_photoflare_qt" ]] && chmod 755 "$STAGING_DIR/usr/bin/gmic_photoflare_qt"
echo ""

# Build .deb
echo "=== Building .deb ==="
dpkg-deb --build --root-owner-group "$STAGING_DIR" "$REPO_ROOT/$DEB_NAME"

echo ""
echo "Built: $REPO_ROOT/$DEB_NAME"
echo ""
echo "To install:"
echo "  sudo dpkg -i $DEB_NAME"
echo "  sudo apt-get install -f    # resolve any missing runtime dependencies"
