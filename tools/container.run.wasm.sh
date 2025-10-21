#!/bin/bash
podman run --rm -v "$(pwd)":/workspace -w /workspace esobinaria bash -c "source /emsdk/emsdk_env.sh && ./tools/build.wasm.sh"
