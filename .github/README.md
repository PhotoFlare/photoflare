# PhotoFlare

[![License: GPL v3+](https://img.shields.io/badge/License-GPL-yellowgreen.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.org/PhotoFlare/photoflare.svg?branch=master)](https://travis-ci.org/PhotoFlare/photoflare)
[![GitHub release](https://img.shields.io/badge/Release-1.5.9-green.svg)](https://github.com/PhotoFlare/photoflare/releases)
[![latest packaged version(s)](https://repology.org/badge/latest-versions/photoflare.svg)](https://repology.org/project/photoflare/versions)
[![Snap Status](https://build.snapcraft.io/badge/PhotoFlare/photoflare.svg)](https://build.snapcraft.io/user/PhotoFlare/photoflare)

Simple but powerful Cross Platform image editor originally inspired by the PhotoFiltre image editor on Windows. Created with C++ 17, Qt Framework and the Graphicsmagick library.

<img src="https://photoflare.io/wp-content/uploads/2018/03/CrossPlatform2.png">

## Documentation
Information on features and using the project will be <a href="https://photoflare.io/documentation/">available here</a>. (WIP)

## Building

### Dependencies
- G++/Clang on Linux, MSVC/Clang on Windows or Clang on MacOSX
- libgraphicsmagick++-dev
- qt5-default (Ubuntu only)  
**or**
- libc6-dev
- libqt5core5a-dev
- libqt5gui5-dev
- libqt5widgets5-dev

Compile using either by opening the .pro file with Qt Creator or use qmake passing the .pro file as a parameter. Full instructions for each platform <a href="https://photoflare.io/contributing/building-the-source/">available here</a>

## Running

### Dependencies
- libc6
- libgraphicsmagick++-q16-12
- libqt5core5a
- libqt5gui5
- libqt5widgets5
- libqt5printsupport5
- qt5-image-formats-plugins (Optional - Extra file format support)

## Translations
Languages currently available:

- English
- French
- Dutch
- German
- Chinese (TW)
- Czech

If you would like to add to this list then get in touch. Instructions on adding a translation is <a href="https://photoflare.io/contributing/translations/">available here</a>
