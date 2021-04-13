The .sub files are for submitting condor jobs. These files do not take arguments, so a new .sub file is often required to for each variation of the jobs you want to submit. For example, there is a .sub file for submitting pp and a separate .sub file for submiting PbPb events.

To submit a job, simply run `condor_submit xxxx.sub`. 

The submission files included are commented with information for each line, but more information can be found [here](https://batchdocs.web.cern.ch/local/quick.html).
One more important caveat, however, is that condor jobs cannot be submitted from the /eos/ directory. For large poductions, saving the output to your home directory will quickly fill up the 4GB quota. Instead, condor jobs should be submitted from the home directory, but save the output to the /eos/ directory (why this is allowed but not simply letting us submit jobs from /eos/ is a mystery). Saving outputs to the correct directory needs to be taken care of in the executable file (i.e. the shell script).

To see the status of your current job, run `condor_q`. This will also give you the JOB_ID for your job.

To kill you job, run `condor_rm JOB_ID`. Useful for jobs that are submitted that were intended as tests.
