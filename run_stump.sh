g++ -o src/main src/main.cpp src/lexer.cpp src/parser.cpp
if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    ./src/main ./samples/test.stump
else
    echo "Compilation unsuccessful"
    exit 1
fi


# mkdir -p build
# cd build

# cmake ..
# make

# if [ $? -eq 0 ]; then
#     echo "Compilation successful!"
#     ./stump ../samples/test.stump
# else
#     echo "Compilation unsuccessful"
#     exit 1
# fi