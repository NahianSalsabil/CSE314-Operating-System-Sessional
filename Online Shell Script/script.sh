#!/usr/bin/bash

friendfile=$1
placefile=$2
visitedfile=$3

if [[ -f "$friendfile" ]]
then
    n=0
    while read line
    do
        friendarr[$n]=$line
        n=$(( n + 1 ))
    done < $friendfile
fi




if [[ -f "$placefile" ]]
then
    n=0
    while read line
    do
        placearr[$n]=$line
        n=$(( n + 1 ))
    done < $placefile
fi


if [[ -f "$visitedfile" ]]
then
    n=0
    while read line
    do
        visitedarr[$n]=$line
        n=$(( n + 1 ))
    done < $visitedfile
fi


i=0
size="${#placearr[@]}"
while (( i < size ))
do
    placecount[$i]=0
    i=$(( i + 1))
done


for visited in "${visitedarr[@]}"
do
    IFS=','
    read -ra arr <<< "$visited"
    for each in "${arr[@]}"
    do
        count=0
        for place in "${placearr[@]}"
        do  
            if [ $each = "$place" ]
            then 
                (( placecount[$count] = placecount[$count] + 1 ))
            fi
            (( count = $count + 1 ))
        done
        
    done
done

echo "${placecount[*]}"
echo "${#placecount[@]}"

