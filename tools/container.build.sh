#!/bin/bash
set -e
podman build -t esobinaria:latest .
echo 'Container built with tag "esobinaria"'
