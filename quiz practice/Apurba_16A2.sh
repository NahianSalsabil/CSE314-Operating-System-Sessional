#16 A2
 
#!/bin/bash
 
usage()
{
    echo "Please provide valid arguments"
    echo "Usage : $0 directory_name[Optional] page_count"
    exit
}
 
 
check_directory_exist()
{
    #provide 1 argument the directory $1
    directory=$1
    while [ ! -d "$directory" ]
    do
        echo "Input valid directory name!"
        read -r directory
    done
}
 
 
#global variables
declare -A parents
declare -A same_name
a1=()
a2=()
get_relative_path()
{
    local now=$1
    if [[ $now == "$directory" ]]; then
        echo "$now"
        return
    fi
    echo "$(get_relative_path  "${parents[$now]}")/$now"
}
 
put_file_in_directory()
{
    local current_file=$1
    if [ "${same_name[$current_file]}" ]; then
        return
    fi
 
    local path=$3
    local position=-1
    for (( i=0; i<${#current_file}; i++ )); do
        if [[ ${current_file:$i:1} == "." ]]; then
            position=$((i+1))
        fi
    done
    local extension="others"
    if (( position != -1 )); then
        local len=$((${#current_file}-position+1))
        extension=${current_file:$position:$len}
    fi
 
    if [[ $extension != "pdf" ]]; then
        return
    fi
    PageCount=$(pdftk "$current_file" dump_data | grep NumberOfPages | awk '{print $2}')
    echo $PageCount
 
    if (( $PageCount <= $page_count )); then
        return
    fi
    same_name[$current_file]=$PageCount
    cd "$back_to"
    mkdir -p output_dir
    cd "$path"
    local lagbe="$back_to/"
    lagbe+="output_dir"
    cp "$current_file" "$lagbe"
}
 
directory_tree()
{
    cd "$1"
    for f in *
    do
        parents[$f]=$1
        if [ -d "$f" ]; then
            local x=$(($2 + 1))
            directory_tree "$f" $x
        elif [ -f "$f" ]; then
            #first check if this file extension is ok
            #put the file in its directory
                local path
                path=$(get_relative_path "${parents[$f]}")
                put_file_in_directory "$f" "$2" "$path"
        fi
    done
    cd ..
}
 
 
# bubble sort
bubble_sort()
{
    local n=${#a1[@]}
    echo "$n"
    flag=1;
    for (( i = 0; i < $n-1; i++ ))
    do
        flag=0;
        for ((j = 0; j < $n-1-$i; j++ ))
        do
            if [[ ${a1[$j]} -gt ${a1[$j+1]} ]]
            then
                local temp=${a1[$j]};
                a1[$j]=${a1[$j+1]};
                a1[$j+1]=$temp;
 
                local temp=${a2[$j]};
                a2[$j]=${a2[$j+1]};
                a2[$j+1]=$temp;
 
                flag=1;
            fi
        done
 
        if [[ $flag -eq 0 ]]; then
            break;
        fi
    done
}
 
 
if (( $# == 2 )); then
    directory=$1
    page_count=$2
    check_directory_exist "$directory"
    back_to=$PWD
elif (( $# == 1 )); then
    directory=${PWD##*/}
    page_count=$1
    cd ..
    back_to=$PWD
else
    usage
fi
 
echo "Directory name : $directory"
echo "Page count : $page_count"
 
 
#get all the files
directory_tree "$directory" 1
 
idx=0
for i in "${!same_name[@]}"
do
  a1[$idx]=${same_name[$i]}
  a2[$idx]=$i
  idx=$((idx+1))
done
 
bubble_sort
 
echo "${a1[@]}"
echo "${a2[@]}"
 
cd "$back_to"
cd "output_dir"
 
echo $PWD
now=1
for (( i=0;i<$idx;i++ ))
do
    mv -f "${a2[$i]}"  "$now.pdf"
    now=$((now+1))
done
