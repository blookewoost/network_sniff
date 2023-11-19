#!/bin/bash

script_dir="$(dirname "$(readlink -f "$0")")" # directory containing this file

bin_dir="../cpp/raw/_build"
bin_file="raw.o"
py_dir="../python"
data_dir="../data"
data_file="out.json"

handle_invalid_args() {
    echo "Usage: ./sniff.sh <packet_num> <options>"
    echo "Valid options are --rebuild and --capture-only"
    exit 0
}

# Gross hack to make up for directory dependencies of the project.
check_dir() {
    if [ ! $(pwd) == script_dir ]; then
        cd $script_dir
    fi
}

check_data_file() {
    check_dir
    if [ -e "$data_dir/$data_file" ]; then
        rm "$data_dir/$data_file"
    fi
}

rebuild() {
    check_dir
    make -C $bin_dir
}

run_capture() {
    check_dir
    check_data_file
    # Check if a binary exists. If not, build the sniffer.
    if [ ! -e "$bin_dir/$bin_file" ]; then
        rebuild
    fi

    cd $bin_dir
    sudo ./$bin_file $1
}

run_report() {
    check_dir
    cd $py_dir
    python3 run.py
}

# Check for command line arguments
if [ "$#" -gt 0 ]; then
    if [[ "$1" =~ ^[0-9]+$ ]]; then # regexp will match on any valid integer
        packet_num=$1
    else
        handle_invalid_args
    fi

    if [ "$#" -gt 1 ]; then # One of the options was provided.
        if [ "$2" == "--rebuild"]; then
            rebuild
            run_capture $packet_num
            run_report
            exit 0
        elif [ "$2" == "--capture-only" ]; then
            run_capture $packet_num
            exit 0
        fi
    fi

    run_capture $packet_num
    run_report
else
    echo "No arguments were passed"
    handle_invalid_args
fi
