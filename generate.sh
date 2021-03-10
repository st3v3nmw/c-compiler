#!/bin/bash

clear
grammarinator-process antlr/C.g4 -o antlr/fuzzer
grammarinator-generate -p antlr/fuzzer/CUnparser.py -l antlr/fuzzer/CUnlexer.py -d 10 -o tests/generated/test_%d.c -n 1

files=`ls tests/generated/*.c`
for entry in $files
do
  echo $entry
  printf "\n"
  cat $entry
  printf "\nTOKENS\n"
  make o f=$entry
  printf "\n\n\n"
  rm $entry
done