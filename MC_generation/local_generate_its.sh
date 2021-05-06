#! /bin/bash
echo "==============================="
echo "execute this script as follows:"
echo "./generate_its.sh system region nevents"
echo "where:"
echo "system = 1 -> pp"
echo "       = 2 -> Pb-Pb"
echo "region = 1 -> ITS only"
echo "       = 2 -> whole detector"
echo "==============================="
# ----------------------------------------------------------
# Checking input parameters
if [ -z "$1" ]
then
	echo "system argument not specified. Bailing out!"
	exit 0
fi
if [ -z "$2" ]
then
	echo "region argument not specified. Bailing out!"
	exit 0
fi
if [ -z "$3" ]
then
	echo "nevents argument not specified. Bailing out!"
	exit 0
fi
# ----------------------------------------------------------
# Grabbing information from the input parameters
# --------------
# parameter 1
if [ $1 == "1" ]
then
	GENERATOR="pythia8"
	echo "will generate pp events"
elif [ $1 == "2" ]
then
	GENERATOR="pythia8hi"
	echo "will generate Pb-Pb events"
else
	echo "Wrong system argument specified. Bailing out!"
	exit 0
fi
# --------------
# parameter 2
if [ $2 == "1" ]
then
	REGION="Diamond.width[2]=6.;SimCutParams.maxRTracking=70;SimCutParams.maxAbsZTracking=100"
	echo "will transport events in the ITS only"
elif [ $2 == "2" ]
then
	REGION="Diamond.width[2]=6."
	echo "will transport events in the whole detector"
else
        echo "Wrong region argument specified. Bailing out!"
        exit 0
fi
# --------------
# parameter 3
echo "Will generate: "$(($3))" events"
# ----------------------------------------------------------
# 1. MC GENERATION RUN
echo ""
echo "=============================================================================================="
echo "1. MC generation run"
o2-sim -n $(($3)) -m PIPE ITS --configKeyValues $REGION -g $GENERATOR -j 6 | tee sim.log
#see https://github.com/AliceO2Group/AliceO2/blob/dev/doc/DetectorSimulation.md for details
# ----------------------------------------------------------
# 2. HITS TO DIGITS CONVERSION
echo ""
echo "=============================================================================================="
echo "2. Hits to digits conversion"
o2-sim-digitizer-workflow --interactionRate=50000 --shm-segment-size 16000000000 --run | tee dig.log
# ----------------------------------------------------------
# 3. TRACKING
echo ""
echo "=============================================================================================="
echo "3. Tracking"
o2-its-reco-workflow --trackerCA --tracking-mode async --shm-segment-size 16000000000 --run | tee rec.log
