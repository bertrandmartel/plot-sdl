#!/bin/bash

CFLAGS="-I../../release/include" \
LDFLAGS="-L../../release" \
python setup.py build_ext -i