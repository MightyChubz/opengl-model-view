#!/bin/bash

conan install . --output-folder=vendored-libs-debug --build=missing --profile=debug
conan install . --output-folder=vendored-libs-release --build=missing --profile=release
