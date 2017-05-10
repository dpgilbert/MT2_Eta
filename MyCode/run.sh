#!/bin/bash

declare -a SignalSamples=(T1bbbb T1tttt T1qqqq T2bW T2bb T2bt T2cc T2qq)

declare -a BackgroundSamples=(dyjets qcd singletop ttsl ttdl ttg tth ttw ttz tttt twz wjets ww zinv)

for SAMPLE in ${BackgroundSamples[@]}; do
    eval 'nohup nice -n 10 ./MyLooper < UnskimmedRunLists/${SAMPLE}.txt >& logs/${SAMPLE}_log.txt &'
done

#for SAMPLE in ${SignalSamples[@]}; do
#    eval 'nohup nice -n 10 ./MyLooper < SkimmedSignalRunLists/${SAMPLE}_SUSYskimmed1.txt >& logs/${SAMPLE}1_log.txt &'
#    eval 'nohup nice -n 10 ./MyLooper < SkimmedSignalRunLists/${SAMPLE}_SUSYskimmed2.txt >& logs/${SAMPLE}2_log.txt &'
#    eval 'nohup nice -n 10 ./MyLooper < SkimmedSignalRunLists/${SAMPLE}_SUSYskimmed3.txt >& logs/${SAMPLE}3_log.txt &'
#done

