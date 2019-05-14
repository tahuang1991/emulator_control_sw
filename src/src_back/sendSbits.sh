#!/bin/bash



times=0

if [ $# -eq 3 ]; then
    echo "input parameter num $#"
else 
    echo "error"
    exit
fi

./send_0.exe
for (( a=$1 ; a <= $2 ; a++ ))
do
        echo "start to send pattern to  $a, pattern name $3"
        ./send.exe $a $3
        echo " "
        sleep 1
done
./send_1.exe

echo "end "
