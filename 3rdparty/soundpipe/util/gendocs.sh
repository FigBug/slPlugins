#!/bin/sh

mkdir -p $INTERMEDIATES_PREFIX/docs
cp -r util/css $INTERMEDIATES_PREFIX/docs

> $INTERMEDIATES_PREFIX/docs/modules.txt

for d in modules/data/*.lua
do
    module=$(basename ${d%.*})
    echo "writing $module"
    util/data2html.lua $module > $INTERMEDIATES_PREFIX/docs/$module.html
    echo $module >> $INTERMEDIATES_PREFIX/docs/modules.txt
done

util/gen_index.lua $INTERMEDIATES_PREFIX/docs/modules.txt > $INTERMEDIATES_PREFIX/docs/index.html
