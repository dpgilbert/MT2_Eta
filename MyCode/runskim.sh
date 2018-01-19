#!/bin/bash

VERSION=Unskimmed

declare -a SignalSamples=(T1bbbb T1tttt T1qqqq T2bW T2bb T2bt T2cc T2qq T2tt)

for SAMPLE in ${SignalSamples[@]}; do
    eval 'nohup nice -n 10 ./MyLooper.exe skim < ${VERSION}RunLists/${SAMPLE}.txt >& logs/${SAMPLE}_skim_log.txt &'
done

