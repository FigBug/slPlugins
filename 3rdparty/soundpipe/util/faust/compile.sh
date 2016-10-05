#!/bin/sh

if [ "$#" -eq 0 ]
then
echo "Please specify the name of the DSP module"
exit 1
fi

NAME=$1
faust -cn $NAME -json -lang c -a sp.c $NAME.dsp -o $NAME.c 
go run parse.go $NAME $NAME.dsp.json >> $NAME.c 
