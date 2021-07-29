# Step-by-step on how to use this repository:

## Examples on how to use O2:
ssh into lxplus:
```
ssh usernamer@lxplus.cern.ch
```
Once there, enter the O2 environment:
```
/cvmfs/alice.cern.ch/bin/alienv enter O2/nightly-20210129-1
```
To run a nominal simulation, do, e.g.:
```
o2-sim -n 10 -m PIPE ITS --configKeyValues "Diamond.width[2]=6.;SimCutParams.maxRTracking=70;SimCutParams.maxAbsZTracking=100" -g pythia8 -j 6 | tee sim.log
```
This generates 10 events (as specified by the flag ```-n```) corresponding to proton-proton collisions (as specified by the ```-g pythia8``` flag. To simulate Pb-Pb collisions replace ```pythia8``` with ```pythia8hi```). The flag ```-j 6``` means that we are using 6 cores. The string ```Diamond.width[2]=6.;SimCutParams.maxRTracking=70;SimCutParams.maxAbsZTracking=100``` means that Geant transport will be limited to the ITS region only. For the whole detector, replace that string with ```Diamond.width[2]=6.```.

2. Then we need to do hits-to-digits conversion:
```
o2-sim-digitizer-workflow --interactionRate=50000 --shm-segment-size 16000000000 --run | tee dig.log
```
The interaction rate of 50kHz is default one, so it can be omitted unless you want to change the rate. More details can be found [https://github.com/AliceO2Group/AliceO2/blob/dev/Steer/DigitizerWorkflow/README.md for details](here).
