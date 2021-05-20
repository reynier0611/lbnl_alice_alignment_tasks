#!/bin/bash
job_id=$1
n_subjobs=$2
dir="/eos/user/f/ftorales/its_data/alignment/lbnl_alice_alignment_tasks/MC_genertion/"

for ((i=0; i<n_subjobs; i++))
do
  echo "i: $i"
  cp $dir/macros/CheckTracks.C $dir/$job_id/$i/
  cd $dir/$job_id/$i/
  root -b -l -q CheckTracks.C
  cd $dir/ 
done

hadd -k -f ${job_id}_Check_Tracks.root ${dir}/${job_id}/*/CheckTracks.root
