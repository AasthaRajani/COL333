# Compile the C++ program (assuming the program file is named 'my_program.cpp')
g++ -o my_program1 part1_input.cpp

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed for part1 input"
    exit 3
fi

# Compile the C++ program (assuming the program file is named 'my_program.cpp')
g++ -o my_program2 part1_output.cpp

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed for part1 output"
    exit 3
fi

# Compile the C++ program (assuming the program file is named 'my_program.cpp')
g++ -o my_program3 pat2_ai.cpp

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed for part 2"
    exit 3
fi
