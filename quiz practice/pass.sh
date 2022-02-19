#!/usr/bin/bash

filename=$1

n=0
while read line; do
    n=$((n+1))
    passarray[$n]=$line
done < $filename

echo ${passarray[0]}