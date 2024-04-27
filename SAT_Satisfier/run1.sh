#!/bin/bash

# Check if the user provided an input file argument
if [ $# -ne 1 ]; then
    echo "Usage: $0 <input_file>"
    exit 1
fi

# Extract the input file name from the command line argument
input_file="$1"

# Run the compiled program with the input file as an argument
./my_program1 "$input_file"

# Cleanup - remove the compiled program executable
rm my_program1