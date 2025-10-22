#!/bin/bash
podman run --rm -v "$(pwd)":/$(pwd) -w /$(pwd) -it esobinaria
