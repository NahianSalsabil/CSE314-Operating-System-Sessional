#!/usr/bin/bash

rm -r output
rm output.csv


if (($# == 2)); then
    directory=$1
    infile=$2

    n=0
    while read line; do
        n=$((n+1))
        ARRAY[$n]=$line
    done < $infile

elif (($# == 1)); then
    if [[ -d $1 ]] ; then
        echo "$1 is a directory";
        echo "Enter working directory and input filename as command line argument"
        exit
    elif [[ -f $1 ]]; then 
        infile=$1
        n=0
        while read line || [ -n "$line" ]
        do
            n=$((n+1))
            ARRAY[$n]=$line
        done < $infile
        directory=./
    else
        echo "Enter a valid input file"
        read file
        if [[ -f $file ]]; then
            n=0
            while read line; do
                n=$((n+1))
                ARRAY[$n]=$line
            done < $file
            directory=./
        fi
    fi
else
    echo "Enter working directory and input filename as command line argument"
    exit
fi    

i=0
for line in `find $directory -type f`; do
    FILE[$i]=$line
    i=$((i+1))
done


count=0
for each in "${FILE[@]}"; do

    ignore=0
    filename=${each##*/}
    file_ext=${filename##*.}

    for ext in "${ARRAY[@]}"; do
        if [[ $file_ext = *"$ext"* ]]; then
            ignore=1
        fi
    done

    if [ $ignore -eq 0 ]; then
        if [ $file_ext = $filename ]; then
            
            mkdir -p "output/others"
            cp $each "output/others"

            list="others_list"

            echo $each >> "output/others/$list"

            index=0
            hastype=0
            for type in "${FILETYPE[@]}"; do
                if [ $type = "others" ]; then
                    (( FILECOUNT[$index] = FILECOUNT[$index] + 1 ))
                    hastype=1
                    break
                fi
                (( index = $index + 1 ))
            done
            if [ $hastype -eq 0 ]; then
                FILETYPE[$count]="others"
                FILECOUNT[$count]=1;
                (( count = $count + 1 ))
            fi
            
            
        else
            mkdir -p "output/$file_ext"
            cp $each "output/$file_ext"

            list="${file_ext}_list"

            echo $each >> "output/$file_ext/$list"

            index=0
            hastype=0
            for type in "${FILETYPE[@]}"; do
                if [ $type = "$file_ext" ]; then
                    (( FILECOUNT[$index] = FILECOUNT[$index] + 1 ))
                    hastype=1
                    break
                fi
                (( index = $index + 1 ))
            done
            if [ $hastype -eq 0 ]; then
                FILETYPE[$count]="$file_ext"
                FILECOUNT[$count]=1;
                (( count = $count + 1 ))
            fi
        fi
    fi
    
done

size="${#FILETYPE[@]}"

echo "File Type, File Count" >> "./output.csv"
i=0
while (( i < $size )); do
    echo "${FILETYPE[$i]},${FILECOUNT[$i]}" >> ./output.csv
    (( i++ ))
done

echo "File copied!"