#!/bin/bash

declare -a T1Samples=(T1bbbb T1tttt T1qqqq)

declare -a T2Samples=(T2bW T2bb T2bt T2qq T2tt)

# T2cc has its own mass point

#declare -a BackgroundSamples=(dyjets qcd singletop ttsl ttdl ttg tth ttw ttz tttt twz wjets ww zinv)

#for SAMPLE in ${BackgroundSamples[@]}; do
#    eval 'nohup nice -n 10 ./MyLooper.exe < UnskimmedRunLists/${SAMPLE}.txt >& logs/${SAMPLE}_log.txt &'
#done

for SAMPLE in ${T1Samples[@]}; do
    eval 'nohup nice -n 10 ./MyLooper.exe < SignalRunLists/${SAMPLE}_mg2200_mx200.txt >& logs/${SAMPLE}_mg2200_mx200_log.txt &'
    eval 'nohup nice -n 10 ./MyLooper.exe < SignalRunLists/${SAMPLE}_mg1700_mx1400.txt >& logs/${SAMPLE}_mg1700_mx1400_log.txt &'
done

for SAMPLE in ${T2Samples[@]}; do
    eval 'nohup nice -n 10 ./MyLooper.exe < SignalRunLists/${SAMPLE}_msq800_mx600.txt >& logs/${SAMPLE}_msq800_mx600_log.txt &'
    eval 'nohup nice -n 10 ./MyLooper.exe < SignalRunLists/${SAMPLE}_msq1200_mx200.txt >& logs/${SAMPLE}_msq1200_mx200_log.txt &'
done

eval 'nohup nice -n 10 ./MyLooper.exe < SignalRunLists/T2cc_msq600_mx550.txt >& logs/T2cc_msq600_mx550_log.txt &'

