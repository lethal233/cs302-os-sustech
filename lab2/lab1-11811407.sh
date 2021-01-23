#!/bin/bash

SAVEIFS=$IFS
IFS=$(echo -en "\n\b") # https://superuser.com/questions/31464/looping-through-ls-results-in-bash-shell-script

head=0
tail=0
size=0
path=()
fileCount=0
dirCount=0
directoryPath="$1"
outputPath="$2"
absPath=
if [ ! -e "$outputPath" ]; then
    if [ "${outputPath%/*}" == "${outputPath#/*}" ]; then
        touch "$outputPath"
    else
        mkdir -p "${outputPath%/*}"
        touch "$outputPath"
    fi
fi
:>"$outputPath" # clear the file content
init2(){
    echo "[${1##*/}]" >> "$outputPath"
    FILES=($(ls "$1"))
    for file in ${FILES[@]}
    do
        absFile=$1"/"${file}
        echo "${absFile}" >> "$outputPath"
        if [ -d "${absFile}" ]; then
            dirCount=$((${dirCount}+1))
            enqueue "${absFile}"
        else
            fileCount=$((${fileCount}+1))
        fi
    done
    echo "" >> "$outputPath"
}

enqueue(){
    # (1) 1 -> absolute path string  
    # name[$tail]=${1##*/} # store the directory name
    path[$tail]="$1" # store the absolute path name of the directory
    tail=$((${tail}+1)) # tail++
    size=$((${size}+1)) # size++
}
dequeue(){
    # topName=${name[${head}]} # pop the name of the directory
    topPath=${path[${head}]} # pop the absolute path of the directory
    head=$((${head}+1)) # head++ 
    size=$((${size}-1)) # size--
}
bfs(){
    absPath=$(cd "$1"; pwd)
    init2 "$absPath"
    while [ ${size} -ne 0 ]
    do
        dequeue
        init2 "${topPath}"
    done
}

bfs "$directoryPath"
echo "[Directories Count]:${dirCount}" >> "$outputPath"
echo "[Files Count]:${fileCount}" >> "$outputPath"
IFS=$SAVEIFS
# https://blog.csdn.net/mrqiang9001/article/details/107733911