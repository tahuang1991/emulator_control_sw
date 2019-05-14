#!/bin/bash



times=0


for (( a=0 ; a < $1 ; a++ ))
do
        echo "start to send_d $a"
./send.exe 1 /home/cscdev/Tao/radtests_aysen2/patterns_gen/twomuons_twoDCFEBs_ClctPattern_CFEB0_tmbc.pat
./send.exe 2 /home/cscdev/Tao/radtests_aysen2/patterns_gen/twomuons_twoDCFEBs_ClctPattern_CFEB1_tmbc.pat
	./send_d.exe 
        echo " "
        sleep 1
done
echo "total send_d  $times"
echo "end "
