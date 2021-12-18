#include "Nano.h"
#include "rooutil.h"
#include "cxxopts.h"
#include "Base.h"
#include "Config.h"
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

    bool isUL = (
        ana.input_file_list_tstring.Contains("UL18") or ana.input_file_list_tstring.Contains("UL2018") or
        ana.input_file_list_tstring.Contains("UL17") or ana.input_file_list_tstring.Contains("UL2017") or
        ana.input_file_list_tstring.Contains("UL16") or ana.input_file_list_tstring.Contains("UL2016")
        );

    bool isAPV = ana.input_file_list_tstring.Contains("NanoAODAPVv9") or ana.input_file_list_tstring.Contains("NanoAODAPVv2") or ana.input_file_list_tstring.Contains("HIPM_UL2016") or ana.input_file_list_tstring.Contains("Run2016C-UL2016") or ana.input_file_list_tstring.Contains("Run2016D-UL2016") or ana.input_file_list_tstring.Contains("Run2016E-UL2016");
    gconf.nanoAOD_ver = isUL ? 8 : 0;
    gconf.isAPV = isAPV ? 1 : 0;
    gconf.GetConfigs(nt.year());
    std::cout <<  " gconf.nanoAOD_ver: " << gconf.nanoAOD_ver <<  " gconf.isAPV: " << gconf.isAPV <<  " nt.year(): " << nt.year() <<  std::endl;

    // Variables
    RooUtil::TTreeX tx("variable", "variable");

    tx.createBranch<int>("run");
    tx.createBranch<int>("lumi");
    tx.createBranch<unsigned long long>("evt");
    tx.createBranch<int>("is_ee");
    tx.createBranch<int>("is_mm");
    tx.createBranch<LV>("tag_p4");
    tx.createBranch<LV>("p4");
    tx.createBranch<int>("pass_ttHTightId");
    tx.createBranch<float>("mll");

    tx.createBranch<vector<float>>("Mee_pt0_eta0_pass", false);
    tx.createBranch<vector<float>>("Mee_pt1_eta0_pass", false);
    tx.createBranch<vector<float>>("Mee_pt2_eta0_pass", false);
    tx.createBranch<vector<float>>("Mee_pt3_eta0_pass", false);
    tx.createBranch<vector<float>>("Mee_pt0_eta1_pass", false);
    tx.createBranch<vector<float>>("Mee_pt1_eta1_pass", false);
    tx.createBranch<vector<float>>("Mee_pt2_eta1_pass", false);
    tx.createBranch<vector<float>>("Mee_pt3_eta1_pass", false);
    tx.createBranch<vector<float>>("Mee_pt0_eta2_pass", false);
    tx.createBranch<vector<float>>("Mee_pt1_eta2_pass", false);
    tx.createBranch<vector<float>>("Mee_pt2_eta2_pass", false);
    tx.createBranch<vector<float>>("Mee_pt3_eta2_pass", false);
    tx.createBranch<vector<float>>("Mee_pt0_eta3_pass", false);
    tx.createBranch<vector<float>>("Mee_pt1_eta3_pass", false);
    tx.createBranch<vector<float>>("Mee_pt2_eta3_pass", false);
    tx.createBranch<vector<float>>("Mee_pt3_eta3_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt0_eta0_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt1_eta0_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt2_eta0_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt3_eta0_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt0_eta1_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt1_eta1_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt2_eta1_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt3_eta1_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt0_eta2_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt1_eta2_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt2_eta2_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt3_eta2_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt0_eta3_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt1_eta3_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt2_eta3_pass", false);
    tx.createBranch<vector<float>>("Mmm_pt3_eta3_pass", false);

    tx.createBranch<vector<float>>("Mee_pt0_eta0_fail", false);
    tx.createBranch<vector<float>>("Mee_pt1_eta0_fail", false);
    tx.createBranch<vector<float>>("Mee_pt2_eta0_fail", false);
    tx.createBranch<vector<float>>("Mee_pt3_eta0_fail", false);
    tx.createBranch<vector<float>>("Mee_pt0_eta1_fail", false);
    tx.createBranch<vector<float>>("Mee_pt1_eta1_fail", false);
    tx.createBranch<vector<float>>("Mee_pt2_eta1_fail", false);
    tx.createBranch<vector<float>>("Mee_pt3_eta1_fail", false);
    tx.createBranch<vector<float>>("Mee_pt0_eta2_fail", false);
    tx.createBranch<vector<float>>("Mee_pt1_eta2_fail", false);
    tx.createBranch<vector<float>>("Mee_pt2_eta2_fail", false);
    tx.createBranch<vector<float>>("Mee_pt3_eta2_fail", false);
    tx.createBranch<vector<float>>("Mee_pt0_eta3_fail", false);
    tx.createBranch<vector<float>>("Mee_pt1_eta3_fail", false);
    tx.createBranch<vector<float>>("Mee_pt2_eta3_fail", false);
    tx.createBranch<vector<float>>("Mee_pt3_eta3_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt0_eta0_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt1_eta0_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt2_eta0_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt3_eta0_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt0_eta1_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt1_eta1_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt2_eta1_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt3_eta1_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt0_eta2_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt1_eta2_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt2_eta2_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt3_eta2_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt0_eta3_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt1_eta3_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt2_eta3_fail", false);
    tx.createBranch<vector<float>>("Mmm_pt3_eta3_fail", false);

    tx.createBranch<vector<LV>>("lep_p4s", false);
    tx.createBranch<vector<int>>("lep_pdgids", false);
    tx.createBranch<vector<int>>("lep_idxs", false);

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
                                              if (not (nt.Muon_pt()[imu]        > 30 )) continue;
                                              if (not (fabs(nt.Muon_eta()[imu]) < 2.4)) continue;
                                              tx.pushbackToBranch<LV>("lep_p4s", nt.Muon_p4()[imu]);
                                              tx.pushbackToBranch<int>("lep_pdgids", (-nt.Muon_charge()[imu]) * 13);
                                              tx.pushbackToBranch<int>("lep_idxs", imu);
                                          }

                                          // Select electrons
                                          for (unsigned int iel = 0; iel < nt.Electron_pt().size(); ++iel)
                                          {
                                              // Loose POG ID
                                              if (not (nt.Electron_mvaFall17V2noIso_WPL()[iel])) continue;
                                              if (not (nt.Electron_pt()[iel]        > 30      )) continue;
                                              if (not (fabs(nt.Electron_eta()[iel]) < 2.5     )) continue;
                                              tx.pushbackToBranch<LV>("lep_p4s", nt.Electron_p4()[iel]);
                                              tx.pushbackToBranch<int>("lep_pdgids", (-nt.Electron_charge()[iel]) * 11);
                                              tx.pushbackToBranch<int>("lep_idxs", iel);
                                          }

                                          return true;

                                      }, UNITY);
    ana.cutflow.addCutToLastActiveCut("DileptonSelection",
                                      [&]()
                                      {
                                          if (not (tx.getBranchLazy<vector<LV>>("lep_p4s").size() == 2)) return false;
                                          tx.setBranch<float>("mll", (tx.getBranchLazy<vector<LV>>("lep_p4s")[0]+tx.getBranchLazy<vector<LV>>("lep_p4s")[1]).mass());
                                          tx.setBranch<int>("run", nt.run());
                                          tx.setBranch<int>("lumi", nt.luminosityBlock());
                                          tx.setBranch<unsigned long long>("evt", nt.event());
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
    ana.cutflow.addCutToLastActiveCut("MuonTnPTagProbePairSelection",
                                      [&]()
                                      {
                                          int imu = tx.getBranchLazy<vector<int>>("lep_idxs")[0];
                                          int jmu = tx.getBranchLazy<vector<int>>("lep_idxs")[1];
                                          if (
                                              (fabs(nt.Muon_dxy()[imu]) < 0.02) and
                                              (fabs(nt.Muon_dz()[imu]) < 0.05) and
                                              (fabs(nt.Muon_sip3d()[imu]) < 4) and
                                              (nt.Muon_tightId()[imu]) and
                                              (fabs(nt.Muon_pfIsoId()[imu]) >= 3)
                                             )
                                          {
                                              tx.setBranch<int>("is_ee", false);
                                              tx.setBranch<int>("is_mm", true);
                                              tx.setBranch<LV>("tag_p4", nt.Muon_p4()[imu]);
                                              tx.setBranch<LV>("p4", nt.Muon_p4()[jmu]);
                                              tx.setBranch<int>("pass_ttHTightId", ttH::muonID(jmu, ttH::IDtight, nt.year()));
                                              float pt = tx.getBranchLazy<LV>("p4").pt();
                                              float eta = fabs(tx.getBranchLazy<LV>("p4").eta());
                                              bool pass = tx.getBranchLazy<int>("pass_ttHTightId");
                                              if (pass)
                                              {
                                                  if      (eta >= 0   and eta < 0.9 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta3_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta3_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta3_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta3_pass", tx.getBranchLazy<float>("mll"));
                                              }
                                              else
                                              {
                                                  if      (eta >= 0   and eta < 0.9 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta3_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta3_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta3_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta3_fail", tx.getBranchLazy<float>("mll"));
                                              }
                                              tx.fill();
                                          }
                                          if (
                                              (fabs(nt.Muon_dxy()[jmu]) < 0.02) and
                                              (fabs(nt.Muon_dz()[jmu]) < 0.05) and
                                              (fabs(nt.Muon_sip3d()[jmu]) < 4) and
                                              (nt.Muon_tightId()[jmu]) and
                                              (fabs(nt.Muon_pfIsoId()[jmu]) >= 3)
                                             )
                                          {
                                              tx.setBranch<int>("is_ee", false);
                                              tx.setBranch<int>("is_mm", true);
                                              tx.setBranch<LV>("tag_p4", nt.Muon_p4()[jmu]);
                                              tx.setBranch<LV>("p4", nt.Muon_p4()[imu]);
                                              tx.setBranch<int>("pass_ttHTightId", ttH::muonID(imu, ttH::IDtight, nt.year()));
                                              float pt = tx.getBranchLazy<LV>("p4").pt();
                                              float eta = fabs(tx.getBranchLazy<LV>("p4").eta());
                                              bool pass = tx.getBranchLazy<int>("pass_ttHTightId");
                                              if (pass)
                                              {
                                                  if      (eta >= 0   and eta < 0.9 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta3_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta3_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta3_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta3_pass", tx.getBranchLazy<float>("mll"));
                                              }
                                              else
                                              {
                                                  if      (eta >= 0   and eta < 0.9 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mmm_pt0_eta3_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mmm_pt1_eta3_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mmm_pt2_eta3_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 80            ) tx.pushbackToBranch<float>("Mmm_pt3_eta3_fail", tx.getBranchLazy<float>("mll"));
                                              }
                                              tx.fill();
                                          }
                                          return true;
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
    ana.cutflow.addCutToLastActiveCut("ElecTnPTagProbePairSelection",
                                      [&]()
                                      {
                                          int iel = tx.getBranchLazy<vector<int>>("lep_idxs")[0];
                                          int jel = tx.getBranchLazy<vector<int>>("lep_idxs")[1];
                                          if (
                                              (fabs(nt.Electron_pt()[iel]) > 40) and // additional pt cuts to go above trigger
                                              (fabs(nt.Electron_sip3d()[iel]) < 4) and
                                              (fabs(nt.Electron_mvaFall17V2Iso_WP90()[iel]))
                                             )
                                          {
                                              tx.setBranch<int>("is_ee", true);
                                              tx.setBranch<int>("is_mm", false);
                                              tx.setBranch<LV>("tag_p4", nt.Electron_p4()[iel]);
                                              tx.setBranch<LV>("p4", nt.Electron_p4()[jel]);
                                              tx.setBranch<int>("pass_ttHTightId", ttH::electronID(jel, ttH::IDtight, nt.year()));
                                              float pt = tx.getBranchLazy<LV>("p4").pt();
                                              float eta = fabs(tx.getBranchLazy<LV>("p4").eta());
                                              bool pass = tx.getBranchLazy<int>("pass_ttHTightId");
                                              if (pass)
                                              {
                                                  if      (eta >= 0   and eta < 0.9 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta3_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta3_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta3_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta3_pass", tx.getBranchLazy<float>("mll"));
                                              }
                                              else
                                              {
                                                  if      (eta >= 0   and eta < 0.9 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta3_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta3_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta3_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta3_fail", tx.getBranchLazy<float>("mll"));
                                              }
                                              tx.fill();
                                          }
                                          if (
                                              (fabs(nt.Electron_pt()[jel]) > 40) and // additional pt cuts to go above trigger
                                              (fabs(nt.Electron_sip3d()[jel]) < 4) and
                                              (fabs(nt.Electron_mvaFall17V2Iso_WP90()[jel]))
                                             )
                                          {
                                              tx.setBranch<int>("is_ee", true);
                                              tx.setBranch<int>("is_mm", false);
                                              tx.setBranch<LV>("tag_p4", nt.Electron_p4()[jel]);
                                              tx.setBranch<LV>("p4", nt.Electron_p4()[iel]);
                                              tx.setBranch<int>("pass_ttHTightId", ttH::electronID(iel, ttH::IDtight, nt.year()));
                                              float pt = tx.getBranchLazy<LV>("p4").pt();
                                              float eta = fabs(tx.getBranchLazy<LV>("p4").eta());
                                              bool pass = tx.getBranchLazy<int>("pass_ttHTightId");
                                              if (pass)
                                              {
                                                  if      (eta >= 0   and eta < 0.9 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta0_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta1_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta2_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta3_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta3_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta3_pass", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta3_pass", tx.getBranchLazy<float>("mll"));
                                              }
                                              else
                                              {
                                                  if      (eta >= 0   and eta < 0.9 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0   and eta < 0.9 and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta0_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 0.9 and eta < 1.2 and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta1_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 1.2 and eta < 2.1 and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta2_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 40 and pt < 45) tx.pushbackToBranch<float>("Mee_pt0_eta3_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 45 and pt < 55) tx.pushbackToBranch<float>("Mee_pt1_eta3_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 55 and pt < 80) tx.pushbackToBranch<float>("Mee_pt2_eta3_fail", tx.getBranchLazy<float>("mll"));
                                                  else if (eta >= 2.1               and pt >= 80            ) tx.pushbackToBranch<float>("Mee_pt3_eta3_fail", tx.getBranchLazy<float>("mll"));
                                              }
                                              tx.fill();
                                          }
                                          return true;
                                      }, UNITY);

    ana.histograms.addHistogram("Nlep", 7, 0, 7, [&]() { return tx.getBranchLazy<vector<LV>>("lep_p4s").size(); } );
    ana.histograms.addHistogram("MllFull", 180, 0, 250, [&]() { return tx.getBranch<float>("mll"); } );
    ana.histograms.addHistogram("Mll", 180, 60, 120, [&]() { return tx.getBranch<float>("mll"); } );

    RooUtil::Histograms histograms_Mmm;
    RooUtil::Histograms histograms_Mee;
    histograms_Mmm.addVecHistogram("Mmm_pt0_eta0_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt0_eta0_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt1_eta0_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt1_eta0_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt2_eta0_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt2_eta0_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt3_eta0_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt3_eta0_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt0_eta1_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt0_eta1_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt1_eta1_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt1_eta1_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt2_eta1_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt2_eta1_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt3_eta1_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt3_eta1_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt0_eta2_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt0_eta2_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt1_eta2_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt1_eta2_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt2_eta2_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt2_eta2_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt3_eta2_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt3_eta2_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt0_eta3_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt0_eta3_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt1_eta3_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt1_eta3_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt2_eta3_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt2_eta3_pass"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt3_eta3_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt3_eta3_pass"); } );

    histograms_Mee.addVecHistogram("Mee_pt0_eta0_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt0_eta0_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt1_eta0_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt1_eta0_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt2_eta0_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt2_eta0_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt3_eta0_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt3_eta0_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt0_eta1_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt0_eta1_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt1_eta1_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt1_eta1_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt2_eta1_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt2_eta1_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt3_eta1_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt3_eta1_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt0_eta2_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt0_eta2_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt1_eta2_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt1_eta2_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt2_eta2_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt2_eta2_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt3_eta2_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt3_eta2_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt0_eta3_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt0_eta3_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt1_eta3_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt1_eta3_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt2_eta3_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt2_eta3_pass"); } );
    histograms_Mee.addVecHistogram("Mee_pt3_eta3_pass", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt3_eta3_pass"); } );

    histograms_Mmm.addVecHistogram("Mmm_pt0_eta0_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt0_eta0_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt1_eta0_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt1_eta0_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt2_eta0_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt2_eta0_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt3_eta0_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt3_eta0_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt0_eta1_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt0_eta1_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt1_eta1_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt1_eta1_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt2_eta1_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt2_eta1_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt3_eta1_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt3_eta1_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt0_eta2_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt0_eta2_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt1_eta2_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt1_eta2_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt2_eta2_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt2_eta2_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt3_eta2_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt3_eta2_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt0_eta3_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt0_eta3_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt1_eta3_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt1_eta3_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt2_eta3_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt2_eta3_fail"); } );
    histograms_Mmm.addVecHistogram("Mmm_pt3_eta3_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mmm_pt3_eta3_fail"); } );

    histograms_Mee.addVecHistogram("Mee_pt0_eta0_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt0_eta0_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt1_eta0_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt1_eta0_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt2_eta0_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt2_eta0_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt3_eta0_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt3_eta0_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt0_eta1_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt0_eta1_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt1_eta1_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt1_eta1_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt2_eta1_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt2_eta1_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt3_eta1_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt3_eta1_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt0_eta2_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt0_eta2_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt1_eta2_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt1_eta2_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt2_eta2_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt2_eta2_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt3_eta2_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt3_eta2_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt0_eta3_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt0_eta3_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt1_eta3_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt1_eta3_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt2_eta3_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt2_eta3_fail"); } );
    histograms_Mee.addVecHistogram("Mee_pt3_eta3_fail", 180, 60, 120, [&]() { return tx.getBranchLazy<vector<float>>("Mee_pt3_eta3_fail"); } );

    ana.cutflow.bookCutflows();
    ana.cutflow.bookHistogramsForCutAndBelow(ana.histograms, "MuonTnPTagProbePairSelection");
    ana.cutflow.bookHistogramsForCutAndBelow(ana.histograms, "ElecTnPTagProbePairSelection");
    ana.cutflow.bookHistogramsForCutAndBelow(histograms_Mmm, "MuonTnPTagProbePairSelection");
    ana.cutflow.bookHistogramsForCutAndBelow(histograms_Mee, "ElecTnPTagProbePairSelection");

    while (ana.looper.nextEvent())
    {

        if (result.count("job_index") and result.count("nsplit_jobs"))
        {
            if (ana.looper.getNEventsProcessed() % ana.nsplit_jobs != (unsigned int) ana.job_index)
                continue;
        }

        ana.cutflow.fill();
        tx.clear();
    }

    ana.cutflow.saveOutput();
    tx.write();
    delete ana.output_tfile;
}
