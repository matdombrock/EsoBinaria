#!/bin/bash
podman run --rm -v "$(pwd)":/$(pwd) -w /$(pwd) esobinaria bash ./tools/build.sh 
