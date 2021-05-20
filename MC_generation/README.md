# Submitting Condor Jobs

Hello there! Ok, let's submit a pp job to Condor.
First of all, we will need to edit a line in ```eos_generate_its.sh```.
Update:

```
eos_dir="/eos/user/f/ftorales/its_data/alignment/lbnl_alice_alignment_tasks"
```

to reflect the path to the directory where you would like the generated files to be stored.
Then, edit the condor submission script. For instance, edit: ```pp_its_condor.sub```.
This is what these submission scripts look like:

```
+JobFlavour = "longlunch"

request_cpus = 4

# 4GB RAM
request_memory = 4096

# 80 GB scratch disk
request_disk = 8192000

executable              = eos_generate_its.sh
arguments               = 1 1 80 $(ClusterId) $(ProcId)
#ClusterID = Job ID. ProcID: Subprocess ID, useful for scripting

output                  = job.$(ClusterId).$(ProcId).out
error                   = job.$(ClusterId).$(ProcId).err
log                     = job.$(ClusterId).log

#number of subjobs to submit
queue 20
```

Edit the job flavor to reflect the maximum runtime requested for each job:

- espresso     = 20 minutes
- microcentury = 1 hour
- longlunch    = 2 hours
- workday      = 8 hours
- tomorrow     = 1 day
- testmatch    = 3 days
- nextweek     = 1 week

More details can be found in [https://batchdocs.web.cern.ch/local/submit.html](https://batchdocs.web.cern.ch/local/submit.html).

Now we are ready to submit the job. Execute:

```
condor_submit pp_its_condor_short.sub
```

To check the status of a job do:

```
condor_q
```

and to remove a job do:

```
condor_rm <jobId>
```
