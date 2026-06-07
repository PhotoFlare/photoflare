# Photoflare

[![License: GPL v3+](https://img.shields.io/badge/License-GPL-yellowgreen.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![GitHub release](https://img.shields.io/badge/Release-1.7.1-green.svg)](https://github.com/PhotoFlare/photoflare/releases)
[![latest packaged version(s)](https://repology.org/badge/latest-versions/photoflare.svg)](https://repology.org/project/photoflare/versions)

Photoflare is a powerful yet easy-to-use cross-platform image editor. It suits a wide variety of different tasks and users who value a more nimble workflow. Features include basic image editing capabilities, paint brushes, image filters, colour adjustments and more advanced features such as Batch image processing.

Shoot RAW? [PhotoFlare Lens](https://photoflare.io/lens/) is a free RAW viewer supporting 700+ camera models with non-destructive XMP editing.

Looking for layers, non-destructive editing, and a professional brush engine? Check out [PhotoFlare Studio](https://photoflare.io/studio/).

<img src="https://photoflare.io/wp-content/uploads/2022/03/crossplatform.png">

## Documentation
Information on current features and how to use the application will be 
<a href="https://photoflare.github.io/photoflare/">available here</a>.

## Building

### Dependencies
- g++ (G++/Clang on Linux, MSVC/Clang on Windows or Clang on MacOSX)
- libgraphicsmagick++1-dev
- libomp-dev
- libc6-dev
- libqt6core6
- libqt6gui6
- libqt6widgets6
- qt6-base-dev

#### Ubuntu (24.04)  
`sudo apt-get install g++ libgraphicsmagick++1-dev libc6-dev libqt6core6t64 libqt6gui6t64 libqt6widgets6t64 libomp-dev qt6-base-dev qtcreator`

#### Ubuntu (26.04)
`sudo apt-get install g++ libgraphicsmagick++1-dev libc6-dev libqt6core6 libqt6gui6 libqt6widgets6 libomp-21-dev qt6-base-dev qtcreator`

Compile using either by opening the .pro file with Qt Creator or use qmake passing the .pro file as a parameter. 

`qmake Photoflare.pro`

#### gmic-qt
```bash
git submodule update --init --recursive
bash external/build_gmic_photoflare_qt.sh
```

Full instructions for each platform <a href="https://photoflare.io/contributing/building-the-source/">available here</a>

## Running

### Dependencies
- libc6
- libgraphicsmagick++
- libqt6core6
- libqt6gui6
- libqt6widgets6
- libqt6printsupport6
- libomp
- qt6-image-formats-plugins (Optional - Extra file format support)

## Translations
Languages currently available:

- English
- French (fr)
- Dutch (nl)
- German (de)
- Chinese (Simplified - zh_CN)
- Chinese (Traditional - zh_TW)
- Czech (cs)
- Portuguese (pt_BR)
- Portuguese (pt_PT)
- Swedish (sv)
- Russian (ru_RU)
- Indonesian (id)
- Spanish (es)
- Japanese (ja)
- Turkish (tr)
- Korean (ko)

If you would like to add to this list then get in touch. Instructions on adding a translation is <a href="https://photoflare.io/contributing/translations/">available here</a>
