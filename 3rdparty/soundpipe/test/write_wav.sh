#/bin/sh

# This utility is a wrapper around sox which converts 32-bit LE raw data into a 
# mono 44.1 kHz wav.

if [ "$#" -eq 0 ]
then
    echo "./write_wave.sh filename.raw"
    exit
fi

sox -t raw -c 1 -r 44100 -e floating-point -b 32 $1 out.wav

echo "converted $1 to out.wav"

