#!/bin/bash

#wrapper script for extracting contours and creating temperature plots
#Can pass argument to run ETH or SNT scripts, ETH by default
#Define MODEL, DATE, and INDIR

#the model name
#MODEL="T1tttt"
#MODEL="T1bbbb"
MODEL="T1qqqq"
#MODEL="T2tt"
#MODEL="T2bb"
#MODEL="T2-4bd_scan"

#the suffix
DATE="PseudoJetEta_NoNan"

#directory where cards and points_MODEL.txt can be found
#INDIR="/home/users/mderdzinski/winter2017/clean_master/scripts/cards_${MODEL}_${DATE}"
INDIR="/home/users/dpgilber/MT2AnalysisEdit/scripts/cards_PseudoJetEta_NolnNFix"

LIMITDIR="/hadoop/cms/store/user/${USER}/combine/limits/${MODEL}_${DATE}"

if [ $# == 0 ]
then
    TYPE="ETH"
elif [ $# == 1 ]
then
    TYPE=$1
else
    echo "Illegal number of Parameters."
    kill -INT $$
fi

echo "Running with Parameters:"
echo "MODEL = ${MODEL}"
echo "DATE  = ${DATE}"
echo "TYPE  = ${TYPE}"
echo "INDIR = ${INDIR}"

if [ -e log_makePlots.log ]
then
    rm log_makePlots.log
fi

if [ $TYPE == "ETH" ]
then
    echo "Making ETH scans..."

    if [ -e limits_${MODEL}_${DATE}.root ]
    then
	rm limits_${MODEL}_${DATE}.root
    fi
    
    #call script to convert limits to text file
    echo "python makeLimitFile.py ${MODEL} ${DATE} ${INDIR}/points_${MODEL}.txt"
    python makeLimitFile.py ${MODEL} ${DATE} ${INDIR}/points_${MODEL}.txt >> log_makePlots.log 2>&1

    #take limit text file and feed to ETH plotting script
    echo "python drawSMSlimit.py limits_${MODEL}_${DATE}.txt"
    python drawSMSlimit.py limits_${MODEL}_${DATE}.txt >> log_makePlots.log 2>&1

    #take root file and feed to PlotSMS package
    cp -v limits_${MODEL}_${DATE}.root PlotsSMS/config/mt2/${MODEL}_results.root >> log_makePlots.log 2>&1
elif [ $TYPE == "SNT" ]
then
    echo "Making SNT scans..."

    if [ -e r-values_${MODEL}.root ]
    then
	rm r-values_${MODEL}.root
    fi

    #run make_rValues to fill histograms with limits
    echo "Filling Histograms..."
    echo ".L make_rValues.C+" > temp
    while read i
    do
	MASS1=$(echo "$i"|awk -F- 'split($1,a,"_")&&$0=a[2]')
	MASS2=$(echo "$i"|awk -F- 'split($1,a,"_")&&$0=a[3]')
	if [ "$MASS2" == "" ]
	then
	    MASS2="0"
	fi
	echo "make_rValues(\"$LIMITDIR\",\"$MODEL\",$MASS1,$MASS2)" >> temp
	
    done < $INDIR/points_$MODEL.txt
    echo ".q" >> temp

    cat temp | root -b >> log_makePlots.log 2>&1
    rm temp

    #smooth histograms and make contours
    echo "root -b -q smooth.C(\"$MODEL\")"
    root -b -q "smooth.C(\"$MODEL\")" >> log_makePlots.log 2>&1
    echo "root -b -q make_contour.C(\"$MODEL\")"
    root -b -q "make_contour.C(\"$MODEL\")" >> log_makePlots.log 2>&1

    #take root file and feed to PlotSMS package
    cp -v r-values_${MODEL}.root PlotsSMS/config/mt2/${MODEL}_results.root >> log_makePlots.log 2>&1
else
    echo "${TYPE} is not a valid TYPE!"
    kill -INT $$
fi

echo "Running PlotSMS package..."
pushd PlotsSMS >> log_makePlots.log 2>&1
python python/makeSMSplots.py config/mt2/${MODEL}_mt2.cfg $MODEL >> log_makePlots.log 2>&1
popd >> log_makePlots.log 2>&1

echo "Done."
