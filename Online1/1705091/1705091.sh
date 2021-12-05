#!/usr/bin/bash

inputfile=$1

if [[ -f "$inputfile" ]]
then
    n=0
    while read line || [ -n "$line" ]
    do
        inputarr[$n]=$line
        n=$(( n + 1 ))
    done < $inputfile
fi
echo "${inputarr[*]}"


count=0
for line in "${inputarr[@]}"
do
    IFS=','
    read -ra arr <<< "$line"
    if ! [ $count -eq 0 ]
    then
       studentid[(($count-1))]=${arr[0]}
       degree[(($count-1))]=${arr[1]}
       inst[(($count-1))]=${arr[2]}
    fi
    (( count = $count + 1 ))
done

echo "${studentid[*]}"
echo "${degree[*]}"
echo "${inst[*]}"


size="${#studentid[@]}"
i=0
echo "$size"

echo "Student Id, Degree, Institute" >> ./output.csv
for each in "${studentid[@]}"
do
    echo "----"
    echo "$each"
    degreecount=1

    (( j = $i + 1 ))
    while (( j < $size))
    do  
        st1=$studentid[$i]
        echo "st1"
        echo "$st1"
        if [[ ${studentid[$i]} = ${studentid[$j]} ]]
        then 
            echo "${studentid[$i]}"
            (( degreecount = $degreecount + 1 ))
            $studentid[$i]=-1
        fi
        (( j = $j + 1 ))
    done
    echo "/////"
    
    (( i = $i + 1 ))
    echo "${studentid[i]}, $degreecount" >> ./output.csv
done


