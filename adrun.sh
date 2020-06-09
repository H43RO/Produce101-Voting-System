#!/bin/bash

cd build
cmake ..
make install
cd ../bin
./Admin_exe
echo "iVote off"
cd ..
if [ ! -d "Board_data" ]
then
    mkdir Board_data
fi

if [ -e "Board_data" ]
then
    rm Board_data/*.txt
fi

mv bin/board_*.txt Board_data
clear