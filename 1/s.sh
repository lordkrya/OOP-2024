#!/bin/bash

cmake . -BbuildL -DSHARED_LIBS=TRUE -DSEARCH_MOD=TRUE -DTESTING_MOD=TRUE
cmake --build buildL --target lab1cpp wordSearch_testing
