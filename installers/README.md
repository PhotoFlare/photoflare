# PhotoFlare installers

Sources and scripts for building the PhotoFlare distribution packages. Each package also has a GitHub Actions workflow in [`.github/workflows/`](../.github/workflows). Workflows triggered by a `v*` tag push attach their artifacts to the GitHub Release; all can also be run manually (`workflow_dispatch`).

| Package | Local build | CI workflow | Notes |
|---------|-------------|-------------|-------|
| Windows installer (.msi) | `Photoflare.aip` (Advanced Installer) | `build-and-sign-win-installer.yml` | Manual trigger only. |
| Windows portable | n/a | `build-and-sign-win-portable.yml` | Signed. |
| Debian/Ubuntu (.deb) | `build_deb.sh` | `build-deb.yml` | Built for x86_64 and ARM. |
| AppImage | `build_appimage.sh` | `appimage.yml` | |
| Flatpak | `build_flatpak.sh` | `build-flatpak.yml` | Manifest in `../flatpak/`. |
| Snap | `build_snap.sh` | `build-snap.yml` | Config in `../snap/snapcraft.yaml`. |
| macOS (.dmg) | n/a | `build-macos-dmg.yml` | |

## Contents

- `Photoflare.aip`: Advanced Installer project for the Windows installer. The product version is read from `photoflare.exe`.
- `deb/DEBIAN/`: `control` / `control_simple` files and desktop/icon assets used for the .deb.
- `build_*.sh`: Linux helper scripts (see below). They `cd` to the repo root, so run them from anywhere.
- `assets/`: Windows installer assets (EULA, icon).

## Local builds (Linux)

Each script documents its prerequisites in its header. Outputs are written to the repo root.

```bash
bash installers/build_deb.sh [--prefix /path/to/qt6] [--with-gmic]
bash installers/build_appimage.sh [--version X] [--prefix /path/to/qt6]
bash installers/build_flatpak.sh [--install] [--run]
bash installers/build_snap.sh [--install] [--run]
```

## Release checklist

Bump the version in all of these:

- `src/main.cpp` (`setApplicationVersion`)
- `io.photoflare.photoflare.appdata.xml` (new `<release>` entry)
- `snap/snapcraft.yaml`
- `installers/deb/DEBIAN/control` and `control_simple`
- `installers/Photoflare.aip` (`photoflare.exe` file version)
- `photoflare.1` (version and date)
- `.github/README.md` (release badge)
- `versions/dev.json`; `versions/stable.json` **only after** the release is published

Then:

1. Update translations (`python3 languages/diffing_tool.py`) and build/smoke-test on each platform.
2. Commit and push, then tag `vX.Y.Z` and push the tag.
3. Confirm the tag-triggered workflows succeed and attached their artifacts to the release.
4. Manually run the Windows installer workflow and attach the signed installer.
5. Publish the release notes, update external listings (Flathub, Snap Store, website), then bump `versions/stable.json`.
