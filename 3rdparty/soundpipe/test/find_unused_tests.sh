#!/bin/sh

# this program figures out which tests haven't been implemented yet.

ls t/*.c > files
cat all_tests.h | sed "s/TEST(\(.*\),.*,.*)/t\/\1.c/" >> files
cat files | sort | uniq -u

rm files
