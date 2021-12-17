#include "Nano.h"
#include "rooutil.h"
#include "cxxopts.h"
#include "Base.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"

class AnalysisConfig {

public:

    // TString that holds the input file list (comma separated)
    TString input_file_list_tstring;

    // TString that holds the name of the TTree to open for each input files
    TString input_tree_name;

    // Output TFile
    TFile* output_tfile;

    // Number of events to loop over
    int n_events;

    // Jobs to split (if this number is positive, then we will skip certain number of events)
    // If there are N events, and was asked to split 2 ways, then depending on job_index, it will run over first half or latter half
    int nsplit_jobs;

    // Job index (assuming nsplit_jobs is set, the job_index determine where to loop over)
    int job_index;

    // Debug boolean
    bool debug;

    // TChain that holds the input TTree's
    TChain* events_tchain;

    // Custom Looper object to facilitate looping over many files
    RooUtil::Looper<Nano> looper;

    // Custom Cutflow framework
    RooUtil::Cutflow cutflow;

    // Custom Histograms object compatible with RooUtil::Cutflow framework
    RooUtil::Histograms histograms;

};

AnalysisConfig ana;

// ./process INPUTFILEPATH OUTPUTFILE [NEVENTS]
int main(int argc, char** argv)
{

//********************************************************************************
//
// 1. Parsing options
//
//********************************************************************************

    // cxxopts is just a tool to parse argc, and argv easily

    // Grand option setting
    cxxopts::Options options("\n  $ doAnalysis",  "\n         **********************\n         *                    *\n         *       Looper       *\n         *                    *\n         **********************\n");

    // Read the options
    options.add_options()
        ("i,input"       , "Comma separated input file list OR if just a directory is provided it will glob all in the directory BUT must end with '/' for the path", cxxopts::value<std::string>())
        ("t,tree"        , "Name of the tree in the root file to open and loop over"                                             , cxxopts::value<std::string>())
        ("o,output"      , "Output file name"                                                                                    , cxxopts::value<std::string>())
        ("n,nevents"     , "N events to loop over"                                                                               , cxxopts::value<int>()->default_value("-1"))
        ("j,nsplit_jobs" , "Enable splitting jobs by N blocks (--job_index must be set)"                                         , cxxopts::value<int>())
        ("I,job_index"   , "job_index of split jobs (--nsplit_jobs must be set. index starts from 0. i.e. 0, 1, 2, 3, etc...)"   , cxxopts::value<int>())
        ("d,debug"       , "Run debug job. i.e. overrides output option to 'debug.root' and 'recreate's the file.")
        ("h,help"        , "Print help")
        ;

    auto result = options.parse(argc, argv);

    // NOTE: When an option was provided (e.g. -i or --input), then the result.count("<option name>") is more than 0
    // Therefore, the option can be parsed easily by asking the condition if (result.count("<option name>");
    // That's how the several options are parsed below

    //_______________________________________________________________________________
    // --help
    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(1);
    }

    //_______________________________________________________________________________
    // --input
    if (result.count("input"))
    {
        ana.input_file_list_tstring = result["input"].as<std::string>();
    }
    else
    {
        std::cout << options.help() << std::endl;
        std::cout << "ERROR: Input list is not provided! Check your arguments" << std::endl;
        exit(1);
    }

    //_______________________________________________________________________________
    // --tree
    if (result.count("tree"))
    {
        ana.input_tree_name = result["tree"].as<std::string>();
    }
    else
    {
        std::cout << options.help() << std::endl;
        std::cout << "ERROR: Input tree name is not provided! Check your arguments" << std::endl;
        exit(1);
    }

    //_______________________________________________________________________________
    // --debug
    if (result.count("debug"))
    {
        ana.output_tfile = new TFile("debug.root", "recreate");
    }
    else
    {
        //_______________________________________________________________________________
        // --output
        if (result.count("output"))
        {
            ana.output_tfile = new TFile(result["output"].as<std::string>().c_str(), "create");
            if (not ana.output_tfile->IsOpen())
            {
                std::cout << options.help() << std::endl;
                std::cout << "ERROR: output already exists! provide new output name or delete old file. OUTPUTFILE=" << result["output"].as<std::string>() << std::endl;
                exit(1);
            }
        }
        else
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: Output file name is not provided! Check your arguments" << std::endl;
            exit(1);
        }
    }

    //_______________________________________________________________________________
    // --nevents
    ana.n_events = result["nevents"].as<int>();

    //_______________________________________________________________________________
    // --nsplit_jobs
    if (result.count("nsplit_jobs"))
    {
        ana.nsplit_jobs = result["nsplit_jobs"].as<int>();
        if (ana.nsplit_jobs <= 0)
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --nsplit_jobs" << ana.nsplit_jobs << " has zero or negative value!" << std::endl;
            std::cout << "I am not sure what this means..." << std::endl;
            exit(1);
        }
    }
    else
    {
        ana.nsplit_jobs = -1;
    }

    //_______________________________________________________________________________
    // --nsplit_jobs
    if (result.count("job_index"))
    {
        ana.job_index = result["job_index"].as<int>();
        if (ana.job_index < 0)
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --job_index" << ana.job_index << " has negative value!" << std::endl;
            std::cout << "I am not sure what this means..." << std::endl;
            exit(1);
        }
    }
    else
    {
        ana.job_index = -1;
    }


    // Sanity check for split jobs (if one is set the other must be set too)
    if (result.count("job_index") or result.count("nsplit_jobs"))
    {
        // If one is not provided then throw error
        if ( not (result.count("job_index") and result.count("nsplit_jobs")))
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --job_index and --nsplit_jobs must be set at the same time!" << std::endl;
            exit(1);
        }
        // If it is set then check for sanity
        else
        {
            if (ana.job_index >= ana.nsplit_jobs)
            {
                std::cout << options.help() << std::endl;
                std::cout << "ERROR: --job_index >= --nsplit_jobs ! This does not make sense..." << std::endl;
                exit(1);
            }
        }
    }

    //
    // Printing out the option settings overview
    //
    std::cout <<  "=========================================================" << std::endl;
    std::cout <<  " Setting of the analysis job based on provided arguments " << std::endl;
    std::cout <<  "---------------------------------------------------------" << std::endl;
    std::cout <<  " ana.input_file_list_tstring: " << ana.input_file_list_tstring <<  std::endl;
    std::cout <<  " ana.output_tfile: " << ana.output_tfile->GetName() <<  std::endl;
    std::cout <<  " ana.n_events: " << ana.n_events <<  std::endl;
    std::cout <<  " ana.nsplit_jobs: " << ana.nsplit_jobs <<  std::endl;
    std::cout <<  " ana.job_index: " << ana.job_index <<  std::endl;
    std::cout <<  "=========================================================" << std::endl;

//********************************************************************************
//
// 2. Opening input baby files
//
//********************************************************************************

    // Create the TChain that holds the TTree's of the baby ntuples
    ana.events_tchain = RooUtil::FileUtil::createTChain(ana.input_tree_name, ana.input_file_list_tstring);

    ana.looper.init(ana.events_tchain, &nt, ana.n_events);

    // Variables
    RooUtil::TTreeX tx("variable", "variable");

    tx.createBranch<int>("is_ee");
    tx.createBranch<int>("is_mm");
    tx.createBranch<LV>("tag_p4");
    tx.createBranch<LV>("p4");
    tx.createBranch<int>("pass_id");
    tx.createBranch<float>("mll");

    tx.createBranch<vector<LV>>("lep_p4s");
    tx.createBranch<vector<int>>("lep_pdgids");

    ana.cutflow.setTFile(ana.output_tfile);
    ana.cutflow.addCut("Weight", UNITY, UNITY);
    ana.cutflow.addCutToLastActiveCut("SelectLeptons",
                                      [&]()
                                      {
                                          // Select muons
                                          for (unsigned int imu = 0; imu < nt.Muon_pt().size(); ++imu)
                                          {
                                              // Loose POG ID
                                              if (not (nt.Muon_looseId()[imu]        )) continue;
                                              if (not (nt.Muon_pt()[imu]        > 40 )) continue;
                                              if (not (fabs(nt.Muon_eta()[imu]) < 2.4)) continue;
                                              tx.pushbackToBranch<LV>("lep_p4s", nt.Muon_p4()[imu]);
                                              tx.pushbackToBranch<int>("lep_pdgids", (-nt.Muon_charge()[imu]) * 13);
                                          }

                                          // Select electrons
                                          for (unsigned int iel = 0; iel < nt.Electron_pt().size(); ++iel)
                                          {
                                              // Loose POG ID
                                              if (not (nt.Electron_mvaFall17V2noIso_WPL()[iel])) continue;
                                              if (not (nt.Electron_pt()[iel]        > 40      )) continue;
                                              if (not (fabs(nt.Electron_eta()[iel]) < 2.5     )) continue;
                                              tx.pushbackToBranch<LV>("lep_p4s", nt.Electron_p4()[iel]);
                                              tx.pushbackToBranch<int>("lep_pdgids", (-nt.Electron_charge()[iel]) * 11);
                                          }

                                          return true;

                                      }, UNITY);
    ana.cutflow.addCutToLastActiveCut("DileptonSelection",
                                      [&]()
                                      {
                                          if (not (tx.getBranchLazy<vector<LV>>("lep_p4s").size() == 2)) return false;
                                          tx.setBranch<float>("mll", (tx.getBranchLazy<vector<LV>>("lep_p4s")[0]+tx.getBranchLazy<vector<LV>>("lep_p4s")[1]).mass());
                                          if (not (fabs(tx.getBranch<float>("mll") - 90) < 30)) return false;
                                          return true;
                                      }, UNITY);
    ana.cutflow.getCut("DileptonSelection");
    ana.cutflow.addCutToLastActiveCut("MuonTnPPreselection",
                                      [&]()
                                      {
                                          if (not (tx.getBranchLazy<vector<int>>("lep_pdgids")[0]*tx.getBranchLazy<vector<int>>("lep_pdgids")[1] == -169)) return false;
                                          return true;
                                      }, UNITY);
    ana.cutflow.addCutToLastActiveCut("MuonTnPTriggerSelection",
                                      [&]()
                                      {
                                          bool Common_HLT_IsoMu24   = false;
                                          bool Common_HLT_IsoTkMu24 = false;
                                          bool Common_HLT_IsoMu27   = false;
                                          try { Common_HLT_IsoMu24   = nt.HLT_IsoMu24();   } catch (std::runtime_error) { Common_HLT_IsoMu24   = false; }
                                          try { Common_HLT_IsoTkMu24 = nt.HLT_IsoTkMu24(); } catch (std::runtime_error) { Common_HLT_IsoTkMu24 = false; }
                                          try { Common_HLT_IsoMu27   = nt.HLT_IsoMu27();   } catch (std::runtime_error) { Common_HLT_IsoMu27   = false; }
                                          bool trig_sm = false;
                                          switch (nt.year())
                                          {
                                              case 2016:
                                                  trig_sm = Common_HLT_IsoMu24 or Common_HLT_IsoTkMu24;
                                                  break;
                                              case 2017:
                                                  trig_sm = Common_HLT_IsoMu27;
                                                  break;
                                              case 2018:
                                                  trig_sm = Common_HLT_IsoMu24;
                                                  break;
                                          }
                                          return trig_sm;
                                      }, UNITY);
    ana.cutflow.getCut("DileptonSelection");
    ana.cutflow.addCutToLastActiveCut("ElecTnPPreselection",
                                      [&]()
                                      {
                                          if (not (tx.getBranchLazy<vector<int>>("lep_pdgids")[0]*tx.getBranchLazy<vector<int>>("lep_pdgids")[1] == -121)) return false;
                                          return true;
                                      }, UNITY);
    ana.cutflow.addCutToLastActiveCut("ElecTnPTriggerSelection",
                                      [&]()
                                      {
                                          bool Common_HLT_Ele27_WPTight_Gsf        = false;
                                          bool Common_HLT_Ele25_eta2p1_WPTight_Gsf = false;
                                          bool Common_HLT_Ele35_WPTight_Gsf        = false;
                                          bool Common_HLT_Ele32_WPTight_Gsf        = false;
                                          try { Common_HLT_Ele27_WPTight_Gsf        = nt.HLT_Ele27_WPTight_Gsf();        } catch (std::runtime_error) { Common_HLT_Ele27_WPTight_Gsf        = false; }
                                          try { Common_HLT_Ele25_eta2p1_WPTight_Gsf = nt.HLT_Ele25_eta2p1_WPTight_Gsf(); } catch (std::runtime_error) { Common_HLT_Ele25_eta2p1_WPTight_Gsf = false; }
                                          try { Common_HLT_Ele35_WPTight_Gsf        = nt.HLT_Ele35_WPTight_Gsf();        } catch (std::runtime_error) { Common_HLT_Ele35_WPTight_Gsf        = false; }
                                          try { Common_HLT_Ele32_WPTight_Gsf        = nt.HLT_Ele32_WPTight_Gsf();        } catch (std::runtime_error) { Common_HLT_Ele32_WPTight_Gsf        = false; }
                                          bool trig_se = false;
                                          switch (nt.year())
                                          {
                                              case 2016:
                                                  trig_se = Common_HLT_Ele27_WPTight_Gsf or Common_HLT_Ele25_eta2p1_WPTight_Gsf;
                                                  break;
                                              case 2017:
                                                  trig_se = Common_HLT_Ele35_WPTight_Gsf;
                                                  break;
                                              case 2018:
                                                  trig_se = Common_HLT_Ele32_WPTight_Gsf;
                                                  break;
                                          }
                                          return trig_se;
                                      }, UNITY);

    ana.histograms.addHistogram("Nlep", 7, 0, 7, [&]() { return tx.getBranchLazy<vector<LV>>("lep_p4s").size(); } );
    ana.histograms.addHistogram("MllFull", 180, 0, 250, [&]() { return tx.getBranch<float>("mll"); } );
    ana.histograms.addHistogram("Mll", 180, 60, 120, [&]() { return tx.getBranch<float>("mll"); } );

    ana.cutflow.bookCutflows();
    ana.cutflow.bookHistogramsForCutAndBelow(ana.histograms, "MuonTnPPreselection");
    ana.cutflow.bookHistogramsForCutAndBelow(ana.histograms, "ElecTnPPreselection");

    while (ana.looper.nextEvent())
    {

        if (result.count("job_index") and result.count("nsplit_jobs"))
        {
            if (ana.looper.getNEventsProcessed() % ana.nsplit_jobs != (unsigned int) ana.job_index)
                continue;
        }

        ana.cutflow.fill();
        tx.fill();
        tx.clear();
    }

    ana.cutflow.saveOutput();
    tx.write();
    delete ana.output_tfile;
}
