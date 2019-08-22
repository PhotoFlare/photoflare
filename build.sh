#!/bin/sh -e

# Simple script to build the project on Travis-CI

export QT_SELECT=qt5

apt-get update -q
apt-get install -y qtbase5-dev qttools5-dev-tools libgraphicsmagick++-dev libomp-dev

qmake PhotoFlare.pro
make
# make install # Before to enable it, PhotoFlare.pro need to be adjusted
make clean
