#!/bin/bash

#INDIR=/home/users/jgran/temp/MT2Analysis/MT2looper/output/2015ExtendedNJets

#OUTDIR=cards_2015ExtendedNJets
OUTDIR=cards_test

if [ ! -d "$OUTDIR" ]; then
  mkdir -p $OUTDIR
fi

declare -a samples=("T1tttt_1500_100" "T1tttt_1200_800" "T1bbbb_1500_100" "T1bbbb_1000_900" "T1qqqq_1400_100" "T1qqqq_1000_800" "T2tt_850_100" "T2tt_650_325" "T2tt_500_325" "T2tt_425_325" "T2bb_900_100" "T2bb_600_580" "T2qq_1200_100" "T2qq_600_550")

for i in "${samples[@]}"
do
  nohup root -b -q "cardMaker.C(\"$i\",\"${INDIR}\",\"${OUTDIR}\")" > make_cards_$i.log 2>&1 &
done