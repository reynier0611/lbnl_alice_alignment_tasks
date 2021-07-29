#Step-by-step on how to use this repository:

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
This generates 10 events (as specified by the flag `-n')
