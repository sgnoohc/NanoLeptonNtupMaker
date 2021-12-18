// -*- C++ -*-
// This is a header file generated with the command:
// makeCMS3ClassFiles("/nfs-7/userdata/phchang/tnp_output/v2/DY2018.root", "variable", "LepNtup", "tas", "lep")

#ifndef LepNtup_H
#define LepNtup_H

#include "Math/LorentzVector.h"
#include "Math/GenVector/PtEtaPhiM4D.h"
#include "Math/Point3D.h"
#include "TMath.h"
#include "TBranch.h"
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TBits.h"
#include <vector> 
#include <unistd.h> 
typedef ROOT::Math::LorentzVector< ROOT::Math::PtEtaPhiM4D<float> > LorentzVector;

using namespace std; 
class LepNtup {
 private: 
 protected: 
  unsigned int index;
  int      run_;
  TBranch *run_branch;
  bool     run_isLoaded;
  int      lumi_;
  TBranch *lumi_branch;
  bool     lumi_isLoaded;
  unsigned long long evt_;
  TBranch *evt_branch;
  bool     evt_isLoaded;
  int      is_ee_;
  TBranch *is_ee_branch;
  bool     is_ee_isLoaded;
  int      is_mm_;
  TBranch *is_mm_branch;
  bool     is_mm_isLoaded;
  ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > *tag_p4_;
  TBranch *tag_p4_branch;
  bool     tag_p4_isLoaded;
  ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > *p4_;
  TBranch *p4_branch;
  bool     p4_isLoaded;
  int      pass_ttHTightId_;
  TBranch *pass_ttHTightId_branch;
  bool     pass_ttHTightId_isLoaded;
  float    mll_;
  TBranch *mll_branch;
  bool     mll_isLoaded;
  vector<float> *Mee_pt0_eta0_;
  TBranch *Mee_pt0_eta0_branch;
  bool     Mee_pt0_eta0_isLoaded;
  vector<float> *Mee_pt1_eta0_;
  TBranch *Mee_pt1_eta0_branch;
  bool     Mee_pt1_eta0_isLoaded;
  vector<float> *Mee_pt2_eta0_;
  TBranch *Mee_pt2_eta0_branch;
  bool     Mee_pt2_eta0_isLoaded;
  vector<float> *Mee_pt3_eta0_;
  TBranch *Mee_pt3_eta0_branch;
  bool     Mee_pt3_eta0_isLoaded;
  vector<float> *Mee_pt0_eta1_;
  TBranch *Mee_pt0_eta1_branch;
  bool     Mee_pt0_eta1_isLoaded;
  vector<float> *Mee_pt1_eta1_;
  TBranch *Mee_pt1_eta1_branch;
  bool     Mee_pt1_eta1_isLoaded;
  vector<float> *Mee_pt2_eta1_;
  TBranch *Mee_pt2_eta1_branch;
  bool     Mee_pt2_eta1_isLoaded;
  vector<float> *Mee_pt3_eta1_;
  TBranch *Mee_pt3_eta1_branch;
  bool     Mee_pt3_eta1_isLoaded;
  vector<float> *Mee_pt0_eta2_;
  TBranch *Mee_pt0_eta2_branch;
  bool     Mee_pt0_eta2_isLoaded;
  vector<float> *Mee_pt1_eta2_;
  TBranch *Mee_pt1_eta2_branch;
  bool     Mee_pt1_eta2_isLoaded;
  vector<float> *Mee_pt2_eta2_;
  TBranch *Mee_pt2_eta2_branch;
  bool     Mee_pt2_eta2_isLoaded;
  vector<float> *Mee_pt3_eta2_;
  TBranch *Mee_pt3_eta2_branch;
  bool     Mee_pt3_eta2_isLoaded;
  vector<float> *Mee_pt0_eta3_;
  TBranch *Mee_pt0_eta3_branch;
  bool     Mee_pt0_eta3_isLoaded;
  vector<float> *Mee_pt1_eta3_;
  TBranch *Mee_pt1_eta3_branch;
  bool     Mee_pt1_eta3_isLoaded;
  vector<float> *Mee_pt2_eta3_;
  TBranch *Mee_pt2_eta3_branch;
  bool     Mee_pt2_eta3_isLoaded;
  vector<float> *Mee_pt3_eta3_;
  TBranch *Mee_pt3_eta3_branch;
  bool     Mee_pt3_eta3_isLoaded;
  vector<float> *Mmm_pt0_eta0_;
  TBranch *Mmm_pt0_eta0_branch;
  bool     Mmm_pt0_eta0_isLoaded;
  vector<float> *Mmm_pt1_eta0_;
  TBranch *Mmm_pt1_eta0_branch;
  bool     Mmm_pt1_eta0_isLoaded;
  vector<float> *Mmm_pt2_eta0_;
  TBranch *Mmm_pt2_eta0_branch;
  bool     Mmm_pt2_eta0_isLoaded;
  vector<float> *Mmm_pt3_eta0_;
  TBranch *Mmm_pt3_eta0_branch;
  bool     Mmm_pt3_eta0_isLoaded;
  vector<float> *Mmm_pt0_eta1_;
  TBranch *Mmm_pt0_eta1_branch;
  bool     Mmm_pt0_eta1_isLoaded;
  vector<float> *Mmm_pt1_eta1_;
  TBranch *Mmm_pt1_eta1_branch;
  bool     Mmm_pt1_eta1_isLoaded;
  vector<float> *Mmm_pt2_eta1_;
  TBranch *Mmm_pt2_eta1_branch;
  bool     Mmm_pt2_eta1_isLoaded;
  vector<float> *Mmm_pt3_eta1_;
  TBranch *Mmm_pt3_eta1_branch;
  bool     Mmm_pt3_eta1_isLoaded;
  vector<float> *Mmm_pt0_eta2_;
  TBranch *Mmm_pt0_eta2_branch;
  bool     Mmm_pt0_eta2_isLoaded;
  vector<float> *Mmm_pt1_eta2_;
  TBranch *Mmm_pt1_eta2_branch;
  bool     Mmm_pt1_eta2_isLoaded;
  vector<float> *Mmm_pt2_eta2_;
  TBranch *Mmm_pt2_eta2_branch;
  bool     Mmm_pt2_eta2_isLoaded;
  vector<float> *Mmm_pt3_eta2_;
  TBranch *Mmm_pt3_eta2_branch;
  bool     Mmm_pt3_eta2_isLoaded;
  vector<float> *Mmm_pt0_eta3_;
  TBranch *Mmm_pt0_eta3_branch;
  bool     Mmm_pt0_eta3_isLoaded;
  vector<float> *Mmm_pt1_eta3_;
  TBranch *Mmm_pt1_eta3_branch;
  bool     Mmm_pt1_eta3_isLoaded;
  vector<float> *Mmm_pt2_eta3_;
  TBranch *Mmm_pt2_eta3_branch;
  bool     Mmm_pt2_eta3_isLoaded;
  vector<float> *Mmm_pt3_eta3_;
  TBranch *Mmm_pt3_eta3_branch;
  bool     Mmm_pt3_eta3_isLoaded;
public: 
void Init(TTree *tree);
void GetEntry(unsigned int idx); 
void LoadAllBranches(); 
  const int &run();
  const int &lumi();
  const unsigned long long &evt();
  const int &is_ee();
  const int &is_mm();
  const ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > &tag_p4();
  const ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > &p4();
  const int &pass_ttHTightId();
  const float &mll();
  const vector<float> &Mee_pt0_eta0();
  const vector<float> &Mee_pt1_eta0();
  const vector<float> &Mee_pt2_eta0();
  const vector<float> &Mee_pt3_eta0();
  const vector<float> &Mee_pt0_eta1();
  const vector<float> &Mee_pt1_eta1();
  const vector<float> &Mee_pt2_eta1();
  const vector<float> &Mee_pt3_eta1();
  const vector<float> &Mee_pt0_eta2();
  const vector<float> &Mee_pt1_eta2();
  const vector<float> &Mee_pt2_eta2();
  const vector<float> &Mee_pt3_eta2();
  const vector<float> &Mee_pt0_eta3();
  const vector<float> &Mee_pt1_eta3();
  const vector<float> &Mee_pt2_eta3();
  const vector<float> &Mee_pt3_eta3();
  const vector<float> &Mmm_pt0_eta0();
  const vector<float> &Mmm_pt1_eta0();
  const vector<float> &Mmm_pt2_eta0();
  const vector<float> &Mmm_pt3_eta0();
  const vector<float> &Mmm_pt0_eta1();
  const vector<float> &Mmm_pt1_eta1();
  const vector<float> &Mmm_pt2_eta1();
  const vector<float> &Mmm_pt3_eta1();
  const vector<float> &Mmm_pt0_eta2();
  const vector<float> &Mmm_pt1_eta2();
  const vector<float> &Mmm_pt2_eta2();
  const vector<float> &Mmm_pt3_eta2();
  const vector<float> &Mmm_pt0_eta3();
  const vector<float> &Mmm_pt1_eta3();
  const vector<float> &Mmm_pt2_eta3();
  const vector<float> &Mmm_pt3_eta3();

  static void progress(int nEventsTotal, int nEventsChain);
};

#ifndef __CINT__
extern LepNtup lep;
#endif

namespace tas {
  const int &run();
  const int &lumi();
  const unsigned long long &evt();
  const int &is_ee();
  const int &is_mm();
  const ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > &tag_p4();
  const ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > &p4();
  const int &pass_ttHTightId();
  const float &mll();
  const vector<float> &Mee_pt0_eta0();
  const vector<float> &Mee_pt1_eta0();
  const vector<float> &Mee_pt2_eta0();
  const vector<float> &Mee_pt3_eta0();
  const vector<float> &Mee_pt0_eta1();
  const vector<float> &Mee_pt1_eta1();
  const vector<float> &Mee_pt2_eta1();
  const vector<float> &Mee_pt3_eta1();
  const vector<float> &Mee_pt0_eta2();
  const vector<float> &Mee_pt1_eta2();
  const vector<float> &Mee_pt2_eta2();
  const vector<float> &Mee_pt3_eta2();
  const vector<float> &Mee_pt0_eta3();
  const vector<float> &Mee_pt1_eta3();
  const vector<float> &Mee_pt2_eta3();
  const vector<float> &Mee_pt3_eta3();
  const vector<float> &Mmm_pt0_eta0();
  const vector<float> &Mmm_pt1_eta0();
  const vector<float> &Mmm_pt2_eta0();
  const vector<float> &Mmm_pt3_eta0();
  const vector<float> &Mmm_pt0_eta1();
  const vector<float> &Mmm_pt1_eta1();
  const vector<float> &Mmm_pt2_eta1();
  const vector<float> &Mmm_pt3_eta1();
  const vector<float> &Mmm_pt0_eta2();
  const vector<float> &Mmm_pt1_eta2();
  const vector<float> &Mmm_pt2_eta2();
  const vector<float> &Mmm_pt3_eta2();
  const vector<float> &Mmm_pt0_eta3();
  const vector<float> &Mmm_pt1_eta3();
  const vector<float> &Mmm_pt2_eta3();
  const vector<float> &Mmm_pt3_eta3();
}
#include "rooutil.h"
#endif
