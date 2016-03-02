#!/bin/bash



times=0

if [ $# -eq 3 ]; then
    times=$3
else 
    echo "error"
    exit
fi

./send.exe $1 $2
./send_0.exe
for (( a=0 ; a < $3 ; a++ ))
do
        echo "start to send_d $a"
	./send_d.exe 
        echo " "
        sleep 1
done
echo "total send_d  $times"
echo "end "
