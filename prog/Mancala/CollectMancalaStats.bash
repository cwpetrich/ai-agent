#!/bin/bash

sleep_time=10
sleep_time1=3
#
# Runtime comparisons, same depth, different algorithm
#
for depth in `seq 1 6`; do
    logfile="Mancala-MM-AB-${depth}.out"
    ./RunProg ./MancalaServer -a 2 -d 0 -D 0 -R 1 -S 0 -T 1 -t 0 >& ${logfile} &
    sleep ${sleep_time1}
    ./RunProg ./MancalaAgent -a x -U ${depth} -t 0 >& /dev/null &
    sleep ${sleep_time1}
    ./RunProg ./MancalaAgent -a a -U ${depth} -t 0 >& /dev/null
    sleep ${sleep_time}
done

for depth in `seq 1 6`; do
    logfile="Mancala-MMR-ABR-${depth}.out"
    ./RunProg ./MancalaServer -a 2 -d 0 -D 0 -R 1 -S 0 -T 1 -t 0 >& ${logfile} &
    sleep ${sleep_time1}
    ./RunProg ./MancalaAgent -a X -U ${depth} -t 0 >& /dev/null &
    sleep ${sleep_time1}
    ./RunProg ./MancalaAgent -a A -U ${depth} -t 0 >& /dev/null
    sleep ${sleep_time}
done


#
# Score comparisons, different depth, same algorithm
#
for depth1 in `seq 1 8`; do
    for depth2 in `seq ${depth1} 8`; do
	logfile="Mancala-AB-${depth1}-AB-${depth2}.out"
	./RunProg ./MancalaServer -a 2 -d 0 -D 0 -R 1 -S 0 -T 1 -t 0 >& ${logfile} &
	sleep ${sleep_time1}
	./RunProg ./MancalaAgent -a a -U ${depth1} -t 0 >& /dev/null &
	sleep ${sleep_time1}
	./RunProg ./MancalaAgent -a a -U ${depth2} -t 0 >& /dev/null
	sleep ${sleep_time}
    done
done

for depth1 in `seq 1 8`; do
    for depth2 in `seq ${depth1} 8`; do
	logfile="Mancala-ABR-${depth1}-ABR-${depth2}.out"
	./RunProg ./MancalaServer -a 2 -d 0 -D 0 -R 10 -S 0 -T 1 -t 0 >& ${logfile} &
	sleep ${sleep_time1}
	./RunProg ./MancalaAgent -a A -U ${depth1} -t 0 >& /dev/null &
	sleep ${sleep_time1}
	./RunProg ./MancalaAgent -a A -U ${depth2} -t 0 >& /dev/null
	sleep ${sleep_time}
    done
done

for depth1 in `seq 1 6`; do
    for depth2 in `seq ${depth1} 6`; do
	logfile="Mancala-MM-${depth1}-MM-${depth2}.out"
	./RunProg ./MancalaServer -a 2 -d 0 -D 0 -R 1 -S 0 -T 1 -t 0 >& ${logfile} &
	sleep ${sleep_time1}
	./RunProg ./MancalaAgent -a x -U ${depth1} -t 0 >& /dev/null &
	sleep ${sleep_time1}
	./RunProg ./MancalaAgent -a x -U ${depth2} -t 0 >& /dev/null
	sleep ${sleep_time}
    done
done

for depth1 in `seq 1 6`; do
    for depth2 in `seq ${depth1} 6`; do
	logfile="Mancala-MMR-${depth1}-MMR-${depth2}.out"
	./RunProg ./MancalaServer -a 2 -d 0 -D 0 -R 10 -S 0 -T 1 -t 0 >& ${logfile} &
	sleep ${sleep_time1}
	./RunProg ./MancalaAgent -a X -U ${depth1} -t 0 >& /dev/null &
	sleep ${sleep_time1}
	./RunProg ./MancalaAgent -a X -U ${depth2} -t 0 >& /dev/null
	sleep ${sleep_time}
    done
done
