#!/bin/bash

cut -d "," -f 1 "b2_test.csv" | sort -n | uniq -c > stdcnt.txt

declare -A assocArr

while read line; do
  IFS=' ' read -r -a arr <<< "$line"
  assocArr["${arr[1]}"]="${arr[0]}"
done < "stdcnt.txt"

echo "Student_ID, no_of_degree, Degree, Inst" >> result.csv

sort -n b2_test.csv > b2_sorted.csv

previd=0
while read line; do
  # echo $line
  IFS=',' read -r -a arr <<< "$line"
  echo "Current ID ${arr[0]}"
  echo "Previous ID $previd"
  # echo hello
  if [[ "${arr[0]}" == "Student Id" ]]; then
    continue
  fi
  if [[ ${arr[0]} == $previd ]]; then
    echo ", , ${arr[1]}, ${arr[2]}" >> result.csv
  else
    echo "${arr[0]}, ${assocArr[${arr[0]}]} , ${arr[1]}, ${arr[2]}" >> result.csv
  fi
  previd="${arr[0]}"
done < "b2_sorted.csv"

rm -f stdcnt.txt
rm -f log.txt
