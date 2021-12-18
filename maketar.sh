#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

make clean;
make -j;

git status > gitversion.txt
git rev-parse HEAD >> gitversion.txt
git log >> gitversion.txt
git diff >> gitversion.txt

tar -chJf $DIR/package.tar.xz doAnalysis gitversion.txt

rm gitversion.txt
