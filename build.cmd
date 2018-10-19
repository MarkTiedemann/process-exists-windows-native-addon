@echo off
cl does-exist.cpp /nologo
cl native.cpp /nologo
cl wrap.cpp /nologo
node_modules/.bin/node-gyp build --msvs_version 2015 --loglevel warn -j 2