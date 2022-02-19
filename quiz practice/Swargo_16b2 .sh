#!/bin/bash

outputDir="$HOME/Desktop/SortedFiles"
total_files=0
#Take the working directory name (optional) and input file name as a command-line argument

#Extract Lines(Types) and put in an array
extract_types(){
  n=0
  while read line; do
    #statements
    types[$n]=$line
    n=$((n+1))

  done < $1

}

#Gather all the required files in a separate output directory (created by you outside the root working directory).
#Arguments: Directory File Relative_Path
move_file(){
  to_move="$2"
  dir=$1
  relativePath="${3}/${to_move}"
  foldername=${to_move##*.}
  if [[ $foldername == $to_move ]]; then
    dir="${dir}/others"
    txtFileName="${dir}/desc_others.txt"
  else
    dir="${dir}/${foldername}"
    txtFileName="${dir}/desc_${foldername}.txt"
  fi
  mkdir -p $dir
  filepath="${dir}/${to_move}"

  #Ignore Duplicate Files
  if [[ ! -f $filepath ]]; then
    cp "$to_move" $dir
    echo $relativePath >> $txtFileName
  fi
}

#Arg: Filename Level
print_tree(){
  for (( i = 1; i < $2; i++ )); do
    echo -ne "|    "
  done

  echo "|----$1"
}

level=1
#Recursively, from the root working directory and all of its sub-directories, collect the files whose types are not in the input file.
traverse_working_directory(){
  cd "$1"
  for entry in *
    do
      if [[ -d $entry ]]; then
        relative="${2}/${entry}"
        print_tree "$entry" "$level"
        level=$((level+1))
        traverse_working_directory "$entry" "$relative" "$level"
      elif [[ -f $entry ]]; then
        total_files=$((total_files+1))
        thistype=${entry##*.}

        move_flag=1
        for t in "${types[@]}"
        do
          if [[ $t == $thistype ]]; then
            move_flag=0
          fi
        done

        if [[ $move_flag -eq 1 ]]; then
          print_tree "$entry" "$level"
        fi
      else
        echo "Invalid $2 $entry"
      fi
    done
    cd ..
    level=$((level-1))
}

create_csv(){
  cd $outputDir
  declare -A assocArr

  ignored_files=$total_files
  for dir in *
    do
      typecnt=0
      cd $dir
      for subdir in *
      do
        typecnt=$((typecnt+1))
      done
      typecnt=$((typecnt-1))
      ignored_files=$((ignored_files-typecnt))  #This will result the ignored files.
      assocArr[$dir]=$typecnt
      cd ..
    done

    echo "file_type, no_of_file" >> log.csv

    for key in "${!assocArr[@]}"
    do
      echo "$key, ${assocArr[$key]}" >> log.csv
    done

    echo "ignored, $ignored_files" >> log.csv

}

#====================MAIN FUNCTION=====================================================================================
#If the user does not provide any working directory name, consider your script is lying in the root working directory
if [[ $# -eq 2 ]]; then
  workdir=$1
  in=$2
elif [[ $# -eq 1 ]]; then
  workdir=.
  in=$1
fi

n=0
for var in "$@"
do
  types[$n]=$var
    n=$((n+1))
    echo "$var"
done

echo "${types[*]}"
traverse_working_directory "$workdir" "$workdir" "1"

