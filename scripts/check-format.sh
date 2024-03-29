#!/bin/bash
set -eu

readonly format_command=clang-format-14 

readonly scriptpath=$0;
readonly scriptdir=$(dirname $0)
readonly srcdir=$scriptdir/../ruse
readonly testdir=$scriptdir/../ruse_testing

function main(){
    format_files $srcdir
    format_files $testdir
}

function format_file(){
    local inpfile=$1; shift
    local tmpfile=$(mktemp)
    $format_command  $inpfile > $tmpfile
    if [[ $(diff -q $inpfile $tmpfile) ]]
    then
        echo "reformatting $inpfile"
        mv $tmpfile $inpfile
    fi

    rm -f $tmpfile
}

function format_files(){
    local inpdir=$1; shift
    local source_files=$(find $inpdir -type f -regex ".*\\.hpp$\\|.*\\.cpp$")
    for source_file in $source_files
    do
        if [[ -f "$source_file" ]]
        then
            format_file $source_file
        fi
    done
}


main
