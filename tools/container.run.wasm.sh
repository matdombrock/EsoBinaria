#!/bin/bash
podman run --rm -v "$(pwd)":/$(pwd) -w /$(pwd) esobinaria bash -c "source /emsdk/emsdk_env.sh && ./tools/build.wasm.sh"
