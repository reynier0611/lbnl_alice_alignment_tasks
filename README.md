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
or a newer O2 version:
```
/cvmfs/alice.cern.ch/bin/alienv enter O2/nightly-20210813-2
```
The last argument is the O2 version. For a list of all available versions run:
```
/cvmfs/alice-nightlies.cern.ch/bin/alienv q
```
More details can be found [here](https://alice-doc.github.io/alice-analysis-tutorial/building/precomp.html)

1. To run a nominal simulation, do, e.g.:
```
o2-sim -n 10 -m PIPE ITS --configKeyValues "Diamond.width[2]=6.;SimCutParams.maxRTracking=70;SimCutParams.maxAbsZTracking=100" -g pythia8 -j 6 | tee sim.log
```
This generates 10 events (as specified by the flag ```-n```) corresponding to proton-proton collisions (as specified by the ```-g pythia8``` flag. To simulate Pb-Pb collisions replace ```pythia8``` with ```pythia8hi```). The flag ```-j 6``` means that we are using 6 cores. The string ```Diamond.width[2]=6.;SimCutParams.maxRTracking=70;SimCutParams.maxAbsZTracking=100``` means that Geant transport will be limited to the ITS region only. For the whole detector, replace that string with ```Diamond.width[2]=6.```. More details can be found [here](https://github.com/AliceO2Group/AliceO2/blob/dev/doc/DetectorSimulation.md).

2. Then we need to do hits-to-digits conversion:
```
o2-sim-digitizer-workflow --interactionRate=50000 --shm-segment-size 16000000000 --run | tee dig.log
```
The interaction rate of 50kHz is default one, so it can be omitted unless you want to change the rate. More details can be found [here](https://github.com/AliceO2Group/AliceO2/blob/dev/Steer/DigitizerWorkflow/README.md).

3. Finally, we need the tracking step. Run:
```
o2-its-reco-workflow --trackerCA --tracking-mode async --shm-segment-size 16000000000 --run | tee rec.log
```
This will produce two root files named ```o2clus_its.root``` and ```o2trac_its.root```.
All the scripts above can be run from anywhere inside the O2 environment.

4. Copy the macro ```CheckTracks.C``` (which can be found [here](https://github.com/AliceO2Group/AliceO2/blob/dev/Detectors/ITSMFT/ITS/macros/test/CheckTracks.C)) to the same directory where the previous steps were run. This macro can be used to understand how to access tracks and clusters. Do:
```root -l CheckTracks.C```
This will produce a root file with track information.

## Using this repository:
This repository basically automatizes the previous steps. To run nominal simulations, check out: ```MC_generation```. Tu run simulations with misalignment effects, check out: ```MC_generation_misalignment```. These simulations can be run locally on the login node (see ```local_generate_its.sh```) or on Condor (```eos_generate_its.sh```).

The output file produced by ```CheckTracks.C``` can be analyzed with the code ```analysis/make_plots.cxx```

## Misalignment Effects:
In the generation step: ```o2-sim -n 10 -m PIPE ITS ...```, add ```"align-geom.mCCDB=http://localhost:8080;align-geom.mDetectors=ITS"``` to the ```--configKeyValues```.
