#!/bin/sh

$HOME/Git/build-gkg-2.0-Ubuntu-20.04LTS-x86_64/sdk/src/GkgWMBundleWebGLViewer/GkgWMBundleWebGLViewer \
--docroot . \
--approot . \
--http-port 9090 \
--http-addr 0.0.0.0 $@
