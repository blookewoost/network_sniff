#!/bin/bash

# Directory containing this file.
script_dir="$(dirname "$(readlink -f "$0")")" 

# Relative directories/files
bin_dir="./cpp/raw/_build"
bin_file="raw.o"
py_dir="./python"
data_dir="./data"
data_file="out.json"

# Complain to the user
handle_invalid_args() {
    echo "Usage: ./sniff.sh <packet_num> <options>"
    echo "Valid options are --rebuild and --capture-only"
    exit 1
}

# Check directory and change directory if needed.
check_dir() {
    if [ ! $(pwd) == script_dir ]; then
        cd $script_dir || exit 1
    fi
}

# Check and remove data file if it exists.
check_data_file() {
    check_dir
    if [ -e "$data_dir/$data_file" ]; then
        rm "$data_dir/$data_file"
    fi
}

# Build the binary
rebuild() {
    check_dir
    make -C $bin_dir || exit 1
}

# Run the packet capture. If no binary exists, build.
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

# Run python report.
run_report() {
    check_dir
    cd $py_dir || exit 1
    python3 run.py
}

# Check for command line arguments
if [ "$#" -gt 0 ]; then
    if [[ "$1" =~ ^[0-9]+$ ]]; then # regexp will match on any valid integer
        packet_num="$1"
    else
        handle_invalid_args
    fi

    if [ "$#" -gt 1 ]; then # One of the options was provided.
        if [ "$2" == "--rebuild" ]; then
            rebuild
            run_capture "$packet_num"
            run_report
            exit 0
        elif [ "$2" == "--capture-only" ]; then
            run_capture "$packet_num"
            exit 0
        else
            echo -e "$2 is not a valid argument"
            handle_invalid_args
        fi
    fi

    run_capture "$packet_num"
    run_report
else
    echo "No arguments were passed"
    handle_invalid_args
fi
