import os

from metis.Sample import DBSSample, DirectorySample
from metis.LocalMergeTask import LocalMergeTask
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser
import argparse

from time import sleep
import sys

condorpath = os.path.dirname(os.path.realpath(__file__))

if __name__ == "__main__":

    tag = "v2"

    # Where the merged output will go
    merged_dir = "/nfs-7/userdata/{}/tnp_output/{}".format(os.getenv("USER"),tag)

    # Task summary for printing out msummary
    task_summary = {}

    sample_map = {
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIISummer20UL16NanoAODv9-106X_mcRun2_asymptotic_v17-v1_NANOAODSIM_v2.6_TnP"              , dataset="/DY2016")                  : "DY2016" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL16NanoAODAPVv9-106X_mcRun2_asymptotic_preVFP_v11-v1_NANOAODSIM_v2.6_TnP"     , dataset="/DY2016APV")               : "DY2016APV" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1_NANOAODSIM_v2.6_TnP"                 , dataset="/DY2017")                  : "DY2017" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1_ext1-v1_NANOAODSIM_v2.6_TnP" , dataset="/DY2018")                  : "DY2018" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/EGamma_Run2018A-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                               , dataset="/EGamma2018A")             : "EGamma2018A" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/EGamma_Run2018B-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                               , dataset="/EGamma2018B")             : "EGamma2018B" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/EGamma_Run2018C-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                               , dataset="/EGamma2018C")             : "EGamma2018C" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/EGamma_Run2018D-UL2018_MiniAODv2_NanoAODv9-v3_NANOAOD_v2.6_TnP"                                                                               , dataset="/EGamma2018D")             : "EGamma2018D" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2016B-ver1_HIPM_UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                             , dataset="/SingleEl2016B-ver1-HIPM") : "SingleEl2016APVB-ver1-HIPM" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2016B-ver2_HIPM_UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                             , dataset="/SingleEl2016B-ver2-HIPM") : "SingleEl2016APVB-ver2-HIPM" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2016C-HIPM_UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                  , dataset="/SingleEl2016C-HIPM")      : "SingleEl2016APVC-HIPM" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2016D-HIPM_UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                  , dataset="/SingleEl2016D-HIPM")      : "SingleEl2016APVD-HIPM" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2016E-HIPM_UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                  , dataset="/SingleEl2016E-HIPM")      : "SingleEl2016APVE-HIPM" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2016F-HIPM_UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                  , dataset="/SingleEl2016F-HIPM")      : "SingleEl2016APVF-HIPM" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2016F-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                       , dataset="/SingleEl2016F")           : "SingleEl2016F" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2016G-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                       , dataset="/SingleEl2016G")           : "SingleEl2016G" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2016H-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                       , dataset="/SingleEl2016H")           : "SingleEl2016H" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2017B-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                       , dataset="/SingleEl2017B")           : "SingleEl2017B" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2017C-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                       , dataset="/SingleEl2017C")           : "SingleEl2017C" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2017D-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                       , dataset="/SingleEl2017D")           : "SingleEl2017D" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2017E-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                       , dataset="/SingleEl2017E")           : "SingleEl2017E" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleElectron_Run2017F-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                       , dataset="/SingleEl2017F")           : "SingleEl2017F" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2016B-ver1_HIPM_UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                 , dataset="/SingleMu2016B-ver1-HIPM") : "SingleMu2016APVB-ver1-HIPM" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2016B-ver2_HIPM_UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                 , dataset="/SingleMu2016B-ver2-HIPM") : "SingleMu2016APVB-ver2-HIPM" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2016C-HIPM_UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                      , dataset="/SingleMu2016C-HIPM")      : "SingleMu2016APVC-HIPM" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2016D-HIPM_UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                      , dataset="/SingleMu2016D-HIPM")      : "SingleMu2016APVD-HIPM" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2016E-HIPM_UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                      , dataset="/SingleMu2016E-HIPM")      : "SingleMu2016APVE-HIPM" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2016F-HIPM_UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                      , dataset="/SingleMu2016F-HIPM")      : "SingleMu2016APVF-HIPM" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2016F-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2016F")           : "SingleMu2016F" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2016G-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2016G")           : "SingleMu2016G" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2016H-UL2016_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2016H")           : "SingleMu2016H" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2017B-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2017B")           : "SingleMu2017B" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2017C-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2017C")           : "SingleMu2017C" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2017D-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2017D")           : "SingleMu2017D" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2017E-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2017E")           : "SingleMu2017E" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2017F-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2017F")           : "SingleMu2017F" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2017G-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2017G")           : "SingleMu2017G" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2017H-UL2017_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2017H")           : "SingleMu2017H" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2018A-UL2018_MiniAODv2_NanoAODv9-v2_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2018A")           : "SingleMu2018A" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2018B-UL2018_MiniAODv2_NanoAODv9-v2_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2018B")           : "SingleMu2018B" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2018C-UL2018_MiniAODv2_NanoAODv9-v2_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2018C")           : "SingleMu2018C" ,
                DirectorySample(location="/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v2.6_TnP/SingleMuon_Run2018D-UL2018_MiniAODv2_NanoAODv9-v1_NANOAOD_v2.6_TnP"                                                                           , dataset="/SingleMu2018D")           : "SingleMu2018D" ,
        }


    # Infinite loop until all tasks are complete
    # It will sleep every 10 minutes (600 seconds) and re-check automatically
    while True:

        # Boolean to aggregate whether all tasks are complete
        all_tasks_complete = True

        # Loop over the dataset provided by the user few lines above, and do the Metis magic
        for ds,shortname in sample_map.items():
            # skip_tail = True
            skip_tail = False
            task = CondorTask(
                    sample = ds,
                    files_per_output = 1,
                    output_name = "output.root",
                    tag = tag,
                    condor_submit_params = {
                        "sites": "T2_US_UCSD",
                        "use_xrootd":True,
                        },
                    cmssw_version = "CMSSW_10_2_13",
                    scram_arch = "slc7_amd64_gcc700",
                    input_executable = "{}/condor_executable_metis.sh".format(condorpath), # your condor executable here
                    tarfile = "{}/package.tar.xz".format(condorpath), # your tarfile with assorted goodies here
                    special_dir = "TnP/{}".format(tag), # output files into /hadoop/cms/store/<user>/<special_dir>
                    min_completion_fraction = 0.50 if skip_tail else 1.0,
                    # max_jobs = 10,
            )
            # When babymaking task finishes, fire off a task that takes outputs and merges them locally (hadd)
            # into a file that ends up on nfs (specified by `merged_dir` above)
            merge_task = LocalMergeTask(
                    input_filenames=task.get_outputs(),
                    output_filename="{}/{}.root".format(merged_dir,shortname),
                    ignore_bad = skip_tail,
                    )
            # Straightforward logic
            if not task.complete():
                task.process()
            else:
                if not merge_task.complete():
                    merge_task.process()

            # Aggregate whether all tasks are complete
            all_tasks_complete = all_tasks_complete and task.complete()

            # Set task summary
            task_summary[task.get_sample().get_datasetname()] = task.get_task_summary()

        # Parse the summary and make a summary.txt that will be used to pretty status of the jobs
        os.system("rm web_summary.json")
        webdir="~/public_html/dump/TnPdashboard"
        StatsParser(data=task_summary, webdir=webdir).do()
        os.system("chmod -R 755 {}".format(webdir))
        os.system("msummary -r -i {}/web_summary.json".format(webdir))

        # If all done exit the loop
        if all_tasks_complete:
            print("")
            print("All job finished")
            print("")
            break

        # Neat trick to not exit the script for force updating
        print('Press Ctrl-C to force update, otherwise will sleep for 600 seconds')
        try:
            for i in reversed(range(0, 600)):
                sleep(1) # could use a backward counter to be preeety :)
                sys.stdout.write("\r{} mins {} seconds till updating ...".format(i/60, i%60))
                sys.stdout.flush()
        except KeyboardInterrupt:
            raw_input("Press Enter to force update, or Ctrl-C to quit.")
            print("Force updating...")

