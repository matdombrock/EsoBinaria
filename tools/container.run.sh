#!/bin/bash
podman run --rm -v "$(pwd)":/workspace -w /workspace esobinaria bash ./tools/build.sh 
