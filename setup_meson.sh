#!/bin/bash

cd vendored-libs-debug || exit 1
meson setup --native-file conan_meson_native.ini .. ../build/debug --buildtype debug --warnlevel 3

cd ../vendored-libs-release || exit 1
meson setup --native-file conan_meson_native.ini .. ../build/release --buildtype release --warnlevel 3
