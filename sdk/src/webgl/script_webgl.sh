#!/bin/sh

$HOME/Git/build-gkg-2.0-Ubuntu-20.04LTS-x86_64/sdk/src/webgl/webgl \
--docroot . \
--approot . \
--http-port 4040 \
--http-addr 0.0.0.0 $@
