@echo off
call node_modules/.bin/node-gyp clean --loglevel warn
call node_modules/.bin/node-gyp configure --loglevel warn