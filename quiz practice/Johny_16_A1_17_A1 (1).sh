#16 A1----------------------------------------------------------------------------
tr -d '\r' <places.txt >places.txt.tmp
rm -rf output.txt
{ cat <places.txt;} | while read line; do  # Read like this avoid skipping last line
     echo -n $line" ">>output.txt
     grep -c "$line" visited.csv >>output.txt
done
sort --key=2n --reverse output.txt | tail -n 1 | cut -d' ' -f1



#17 A1----------------------------------------------------------------------------
#!/bin/bash


working_dir=$1
threshold=$2
if [ $# != 2 ];then 
    echo "Error: USAGE: $0 [working_dir] [threshold]" #Error message
    exit 1            #Exit with error
fi

mkdir -p $working_dir/../output_dir/
count=1

for file in $(find $working_dir -type f -not -path '*/\.*' -exec grep -Il '.' {} \; | xargs -L 1 echo)
do
    line=$(wc -l $file | cut -d" " -f1)
    if [ $line -gt $threshold ]; then
        # echo $file" "$line
        #ext=$(echo $file | rev | cut -d'.' -f1 | rev)
        cp "$file" $working_dir/../output_dir/$count
        count=$((count+1))
    fi
done
