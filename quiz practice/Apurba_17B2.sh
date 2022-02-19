#17 B2
#!/bin/bash
 
student_id=()
degree=()
institute=()
declare -A vis
 
bubble_sort()
{
    local n=${#student_id[@]}
    echo "$n"
    flag=1;
    for (( i = 0; i < $n-1; i++ ))
    do
        flag=0;
        for ((j = 0; j < $n-1-$i; j++ ))
        do
            if [[ ${student_id[$j]} -gt ${student_id[$j+1]} ]]
            then
                local temp=${student_id[$j]};
                student_id[$j]=${student_id[$j+1]};
                student_id[$j+1]=$temp;
 
                temp=${degree[$j]};
                degree[$j]=${degree[$j+1]};
                degree[$j+1]=$temp;
 
                temp=${institute[$j]};
                institute[$j]=${institute[$j+1]};
                institute[$j+1]=$temp;
 
                flag=1;
            fi
        done
 
        if [[ $flag -eq 0 ]]; then
            break;
        fi
    done
}
 
 
file_name=$1
echo "File name : $file_name"
idx=0
first=0
 
 
while IFS=, read -r id deg ins ; do
    if(( $first == 0 )); then
        first=1
        touch "output.csv"
        echo "$id,Total number of degrees,$deg,$ins" > "output.csv"
        continue
    fi
    student_id[idx]=${id}
    degree[idx]=${deg}
    institute[idx]=${ins}
    idx=$((idx + 1))
done <"$file_name"
 
bubble_sort
 
echo "${student_id[@]}"
 
echo "${degree[@]}"
 
echo "${institute[@]}"
 
 
for (( i=0;i<${#student_id[@]};i++ ))
do
    if [ "${vis[${student_id[$i]}]}" ]; then
        continue
    fi
    now=${student_id[$i]}
    vis[$now]=1
 
    cnt=0
    for (( j=0;j<${#student_id[@]};j++ ))
    do
        now2=${student_id[$j]}
        if(( $now == $now2 )); then
            cnt=$(($cnt+1))
        fi
    done
 
    echo -n "$now,$cnt," >> "output.csv"
    first=0
 
    for (( j=0;j<${#student_id[@]};j++ ))
    do
        now2=${student_id[$j]}
        if(( $now == $now2 )); then
            if (( $first == 0 )); then
                first=1
            else
                echo -n ",," >> "output.csv"
            fi
            echo "${degree[$j]},${institute[$j]}" >> "output.csv"
        fi
    done
done
