#!/bin/sh

makefile () {
    local NAME=$1
    local FILEPATH=$2
    local EXT=$3

    local FOO="$FILEPATH"foo."$EXT"

    if [ ! -f "$FILEPATH$NAME.$EXT" ]
    then
        echo "creating $FILEPATH$NAME.$EXT"
        cp $FOO $FILEPATH$NAME.$EXT
        sed -i -e s/foo/$NAME/g $FILEPATH$NAME.$EXT 
    else
        echo "Error: file $FILEPATH$NAME.$EXT already exists."
        exit 1
    fi
}

if [ "$#" -ne 1 ]
then
    echo "Usage: make bootstrap MODULE_NAME=name"
    exit 1
fi

MODNAME=$1

# Source
makefile $MODNAME modules/ c
# Header
makefile $MODNAME h/ h
# Documentation
makefile $MODNAME modules/data/ lua
# Example
makefile $MODNAME examples/ex_ c
# Test file
makefile $MODNAME test/t/t_ c
