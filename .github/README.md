# Photoflare

[![License: GPL v3+](https://img.shields.io/badge/License-GPL-yellowgreen.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.org/PhotoFlare/photoflare.svg?branch=master)](https://travis-ci.org/PhotoFlare/photoflare)
[![GitHub release](https://img.shields.io/badge/Release-1.6.4-green.svg)](https://github.com/PhotoFlare/photoflare/releases)
[![latest packaged version(s)](https://repology.org/badge/latest-versions/photoflare.svg)](https://repology.org/project/photoflare/versions)

Photoflare is a cross-platform image editor with an aim to balance between powerful features and a very friendly graphical user interface. It suits a wide variety of different tasks and users who value a more nimble workflow. Features include basic image editing capabilities, paint brushes, image filters, colour adjustments and more advanced features such as Batch image processing.

<img src="https://photoflare.io/wp-content/uploads/2018/03/CrossPlatform2.png">

## Documentation
Information on current features and how to use the application will be 
<a href="https://photoflare.github.io/photoflare/">available here</a>. (WIP)

## Building

### Dependencies
- g++ (G++/Clang on Linux, MSVC/Clang on Windows or Clang on MacOSX)
- libgraphicsmagick++1-dev
- libomp-9-dev
- libc6-dev
- libqt5core5a
- libqt5gui5
- libqt5widgets5

#### Ubuntu (18.04)  
`sudo apt-get install libgraphicsmagick++1-dev, libc6-dev, libqt5core5a, libqt5gui5, libqt5widgets5, libomp-9-dev, qtcreator`

Compile using either by opening the .pro file with Qt Creator or use qmake passing the .pro file as a parameter. 

`qmake Photoflare.pro`

Full instructions for each platform <a href="https://photoflare.io/contributing/building-the-source/">available here</a>

## Running

### Dependencies
- libc6
- libgraphicsmagick++-q16-12
- libqt5core5a
- libqt5gui5
- libqt5widgets5
- libqt5printsupport5
- libomp5
- qt5-image-formats-plugins (Optional - Extra file format support)

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

If you would like to add to this list then get in touch. Instructions on adding a translation is <a href="https://photoflare.io/contributing/translations/">available here</a>
