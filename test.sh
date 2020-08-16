#!/usr/bin/env bash
if [ ! -d out ]; then
    CXX=clang++ \
    meson out
fi
meson install -C out
