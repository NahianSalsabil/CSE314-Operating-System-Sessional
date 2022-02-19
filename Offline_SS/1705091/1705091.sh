#!/usr/bin/bash

rm -r output
rm output.csv


if (($# == 2)); then
    directory=$1
    if [[ -f $2 ]]; then        # if file
        infile=$2

        n=0
        while read line; do    # read line by line
            n=$((n+1))
            ARRAY[$n]=$line
        done < $infile
    else
        echo "Enter a valid input file"
        read file
        if [[ -f $file ]]; then      # if file 
            n=0
            while read line; do    # read line by line
                n=$((n+1))
                ARRAY[$n]=$line
            done < $file
        fi
    fi

elif (($# == 1)); then
    if [[ -d $1 ]] ; then
        echo "$1 is a directory";
        echo "Enter working directory and input filename as command line argument"
        exit
    elif [[ -f $1 ]]; then        # if file
        infile=$1
        n=0
        while read line || [ -n "$line" ]     # read line by line
        do
            n=$((n+1))
            ARRAY[$n]=$line
        done < $infile
        directory=./
    else
        echo "Enter a valid input file"
        read file
        if [[ -f $file ]]; then      # if file
            n=0 
            while read line; do      # read line by line
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
for line in `find $directory -type f`; do      # find all files and save them in an array
    FILE[$i]=$line
    i=$((i+1))
done


count=0
ignore_count=0
for each in "${FILE[@]}"; do

    ignore=0
    filename=${each##*/}    
    file_ext=${filename##*.}  

    for ext in "${ARRAY[@]}"; do              # check if in ignore file
        if [[ $file_ext = *"$ext"* ]]; then      # ignored found
            ignore=1
            (( ignore_count = $ignore_count + 1 ))   # increase ignored file count
        fi
    done

    if [[ $ignore -eq 0 ]]; then             # if not ignored this file
        if [[ $file_ext = $filename ]]; then     # others type of file
            mkdir -p "output/others"

            if ! [[ -f "output/others/$filename" ]]; then   # if file not exist to avoid duplicate file
                cp $each "output/others"                    # copy file to that directory
                list="others_list"                          # make that type of file's list

                echo $each >> "output/others/$list"         # add path to that file
            fi

            

            index=0
            hastype=0
            for type in "${FILETYPE[@]}"; do       # from the second time count of type extension
                if [ $type = "others" ]; then      # if type is recorded before
                    (( FILECOUNT[$index] = FILECOUNT[$index] + 1 ))   # increase count
                    hastype=1
                    break                                            # break if match found
                fi
                (( index = $index + 1 ))      #increase index
            done
            if [ $hastype -eq 0 ]; then       # for first time file count of type others
                FILETYPE[$count]="others"     # add this others type of file
                FILECOUNT[$count]=1;          # add count
                (( count = $count + 1 ))      # increase count
            fi
            
            
        else
            mkdir -p "output/$file_ext"
            if ! [[ -f "output/$file_ext/$filename" ]]; then        # if file not exist to avoid duplicate file
                cp $each "output/$file_ext"                         # copy file to that directory
                list="${file_ext}_list"                             # make that type of file's list

                echo $each >> "output/$file_ext/$list"              # add path to that file
            fi

            

            index=0
            hastype=0
            for type in "${FILETYPE[@]}"; do      # from the second time count of type extension
                if [ $type = "$file_ext" ]; then         # match found
                    (( FILECOUNT[$index] = FILECOUNT[$index] + 1 ))    #increase count
                    hastype=1
                    break                              #break if match found
                fi
                (( index = $index + 1 ))            # increase index
            done
            if [ $hastype -eq 0 ]; then          # for first time file count of type extensions
                FILETYPE[$count]="$file_ext"      # add this new type of file
                FILECOUNT[$count]=1;              # add count
                (( count = $count + 1 ))          # increase count
            fi
        fi
    fi
    
done

# output to csv file

size="${#FILETYPE[@]}"        

echo "File Type, File Count" >> "./output.csv"
i=0
while (( i < $size )); do
    echo "${FILETYPE[$i]},${FILECOUNT[$i]}" >> ./output.csv
    (( i++ ))
done
echo "ignored, $ignore_count" >> ./output.csv

echo "File copied!"