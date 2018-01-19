#!/bin/bash

#INDIR=/home/users/dpgilber/MT2AnalysisEdit/MT2looper/output/run1
INDIR=/home/users/dpgilber/MT2AnalysisEdit/MT2looper/output/PseudoJetEta
#OUTDIR=cards_PseudoJetEta_NolnNFix
OUTDIR=cards_Eta_Debug

## set to 1 if doing full signal scans (instead of benchmark signal points)
DOFULLSCAN=1

if [ ! -d "$OUTDIR" ]; then
  mkdir -p $OUTDIR
fi

#declare -a samples=(T1tttt_1500_100 T1tttt_1200_800 T1bbbb_1500_100 T1bbbb_1000_900 T1qqqq_1400_100 T1qqqq_1000_800 T2tt_850_100 T2tt_500_325 T2bb_900_100 T2bb_600_580 T2qq_1200_100 T2qq_600_550)
#declare -a samples=(T1tttt_1500_100 )
#declare -a samples=(T1tttt T1bbbb T1qqqq T2tt T2bb T2qq)
declare -a samples=(T1qqqq)

for i in "${samples[@]}"
do
  nohup root -b -q "cardMaker.C+(\"$i\",\"${INDIR}\",\"${OUTDIR}\",${DOFULLSCAN})" > logs_cards/make_cards_$i.log 2>&1 &
done
