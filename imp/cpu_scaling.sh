#! /bin/bash

#Stop scaling frequency
#------------------------
#CPU frequency scaling enables the operating system to scale the CPU speed up or 
#down in order to save power. You need to stop this before running any time
#based tests.

for i in /sys/devices/system/cpu/cpu[0-9]
do
    echo performance > $i/cpufreq/scaling_governor
done
