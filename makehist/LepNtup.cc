#include "LepNtup.h"
LepNtup lep;

void LepNtup::Init(TTree *tree) {
  tag_p4_branch = tree->GetBranch("tag_p4");
  if (tag_p4_branch) tag_p4_branch->SetAddress(&tag_p4_);
  p4_branch = tree->GetBranch("p4");
  if (p4_branch) p4_branch->SetAddress(&p4_);

  tree->SetMakeClass(1);

  run_branch = tree->GetBranch("run");
  if (run_branch) run_branch->SetAddress(&run_);
  lumi_branch = tree->GetBranch("lumi");
  if (lumi_branch) lumi_branch->SetAddress(&lumi_);
  evt_branch = tree->GetBranch("evt");
  if (evt_branch) evt_branch->SetAddress(&evt_);
  is_ee_branch = tree->GetBranch("is_ee");
  if (is_ee_branch) is_ee_branch->SetAddress(&is_ee_);
  is_mm_branch = tree->GetBranch("is_mm");
  if (is_mm_branch) is_mm_branch->SetAddress(&is_mm_);
  pass_ttHTightId_branch = tree->GetBranch("pass_ttHTightId");
  if (pass_ttHTightId_branch) pass_ttHTightId_branch->SetAddress(&pass_ttHTightId_);
  mll_branch = tree->GetBranch("mll");
  if (mll_branch) mll_branch->SetAddress(&mll_);
  Mee_pt0_eta0_branch = tree->GetBranch("Mee_pt0_eta0");
  if (Mee_pt0_eta0_branch) Mee_pt0_eta0_branch->SetAddress(&Mee_pt0_eta0_);
  Mee_pt1_eta0_branch = tree->GetBranch("Mee_pt1_eta0");
  if (Mee_pt1_eta0_branch) Mee_pt1_eta0_branch->SetAddress(&Mee_pt1_eta0_);
  Mee_pt2_eta0_branch = tree->GetBranch("Mee_pt2_eta0");
  if (Mee_pt2_eta0_branch) Mee_pt2_eta0_branch->SetAddress(&Mee_pt2_eta0_);
  Mee_pt3_eta0_branch = tree->GetBranch("Mee_pt3_eta0");
  if (Mee_pt3_eta0_branch) Mee_pt3_eta0_branch->SetAddress(&Mee_pt3_eta0_);
  Mee_pt0_eta1_branch = tree->GetBranch("Mee_pt0_eta1");
  if (Mee_pt0_eta1_branch) Mee_pt0_eta1_branch->SetAddress(&Mee_pt0_eta1_);
  Mee_pt1_eta1_branch = tree->GetBranch("Mee_pt1_eta1");
  if (Mee_pt1_eta1_branch) Mee_pt1_eta1_branch->SetAddress(&Mee_pt1_eta1_);
  Mee_pt2_eta1_branch = tree->GetBranch("Mee_pt2_eta1");
  if (Mee_pt2_eta1_branch) Mee_pt2_eta1_branch->SetAddress(&Mee_pt2_eta1_);
  Mee_pt3_eta1_branch = tree->GetBranch("Mee_pt3_eta1");
  if (Mee_pt3_eta1_branch) Mee_pt3_eta1_branch->SetAddress(&Mee_pt3_eta1_);
  Mee_pt0_eta2_branch = tree->GetBranch("Mee_pt0_eta2");
  if (Mee_pt0_eta2_branch) Mee_pt0_eta2_branch->SetAddress(&Mee_pt0_eta2_);
  Mee_pt1_eta2_branch = tree->GetBranch("Mee_pt1_eta2");
  if (Mee_pt1_eta2_branch) Mee_pt1_eta2_branch->SetAddress(&Mee_pt1_eta2_);
  Mee_pt2_eta2_branch = tree->GetBranch("Mee_pt2_eta2");
  if (Mee_pt2_eta2_branch) Mee_pt2_eta2_branch->SetAddress(&Mee_pt2_eta2_);
  Mee_pt3_eta2_branch = tree->GetBranch("Mee_pt3_eta2");
  if (Mee_pt3_eta2_branch) Mee_pt3_eta2_branch->SetAddress(&Mee_pt3_eta2_);
  Mee_pt0_eta3_branch = tree->GetBranch("Mee_pt0_eta3");
  if (Mee_pt0_eta3_branch) Mee_pt0_eta3_branch->SetAddress(&Mee_pt0_eta3_);
  Mee_pt1_eta3_branch = tree->GetBranch("Mee_pt1_eta3");
  if (Mee_pt1_eta3_branch) Mee_pt1_eta3_branch->SetAddress(&Mee_pt1_eta3_);
  Mee_pt2_eta3_branch = tree->GetBranch("Mee_pt2_eta3");
  if (Mee_pt2_eta3_branch) Mee_pt2_eta3_branch->SetAddress(&Mee_pt2_eta3_);
  Mee_pt3_eta3_branch = tree->GetBranch("Mee_pt3_eta3");
  if (Mee_pt3_eta3_branch) Mee_pt3_eta3_branch->SetAddress(&Mee_pt3_eta3_);
  Mmm_pt0_eta0_branch = tree->GetBranch("Mmm_pt0_eta0");
  if (Mmm_pt0_eta0_branch) Mmm_pt0_eta0_branch->SetAddress(&Mmm_pt0_eta0_);
  Mmm_pt1_eta0_branch = tree->GetBranch("Mmm_pt1_eta0");
  if (Mmm_pt1_eta0_branch) Mmm_pt1_eta0_branch->SetAddress(&Mmm_pt1_eta0_);
  Mmm_pt2_eta0_branch = tree->GetBranch("Mmm_pt2_eta0");
  if (Mmm_pt2_eta0_branch) Mmm_pt2_eta0_branch->SetAddress(&Mmm_pt2_eta0_);
  Mmm_pt3_eta0_branch = tree->GetBranch("Mmm_pt3_eta0");
  if (Mmm_pt3_eta0_branch) Mmm_pt3_eta0_branch->SetAddress(&Mmm_pt3_eta0_);
  Mmm_pt0_eta1_branch = tree->GetBranch("Mmm_pt0_eta1");
  if (Mmm_pt0_eta1_branch) Mmm_pt0_eta1_branch->SetAddress(&Mmm_pt0_eta1_);
  Mmm_pt1_eta1_branch = tree->GetBranch("Mmm_pt1_eta1");
  if (Mmm_pt1_eta1_branch) Mmm_pt1_eta1_branch->SetAddress(&Mmm_pt1_eta1_);
  Mmm_pt2_eta1_branch = tree->GetBranch("Mmm_pt2_eta1");
  if (Mmm_pt2_eta1_branch) Mmm_pt2_eta1_branch->SetAddress(&Mmm_pt2_eta1_);
  Mmm_pt3_eta1_branch = tree->GetBranch("Mmm_pt3_eta1");
  if (Mmm_pt3_eta1_branch) Mmm_pt3_eta1_branch->SetAddress(&Mmm_pt3_eta1_);
  Mmm_pt0_eta2_branch = tree->GetBranch("Mmm_pt0_eta2");
  if (Mmm_pt0_eta2_branch) Mmm_pt0_eta2_branch->SetAddress(&Mmm_pt0_eta2_);
  Mmm_pt1_eta2_branch = tree->GetBranch("Mmm_pt1_eta2");
  if (Mmm_pt1_eta2_branch) Mmm_pt1_eta2_branch->SetAddress(&Mmm_pt1_eta2_);
  Mmm_pt2_eta2_branch = tree->GetBranch("Mmm_pt2_eta2");
  if (Mmm_pt2_eta2_branch) Mmm_pt2_eta2_branch->SetAddress(&Mmm_pt2_eta2_);
  Mmm_pt3_eta2_branch = tree->GetBranch("Mmm_pt3_eta2");
  if (Mmm_pt3_eta2_branch) Mmm_pt3_eta2_branch->SetAddress(&Mmm_pt3_eta2_);
  Mmm_pt0_eta3_branch = tree->GetBranch("Mmm_pt0_eta3");
  if (Mmm_pt0_eta3_branch) Mmm_pt0_eta3_branch->SetAddress(&Mmm_pt0_eta3_);
  Mmm_pt1_eta3_branch = tree->GetBranch("Mmm_pt1_eta3");
  if (Mmm_pt1_eta3_branch) Mmm_pt1_eta3_branch->SetAddress(&Mmm_pt1_eta3_);
  Mmm_pt2_eta3_branch = tree->GetBranch("Mmm_pt2_eta3");
  if (Mmm_pt2_eta3_branch) Mmm_pt2_eta3_branch->SetAddress(&Mmm_pt2_eta3_);
  Mmm_pt3_eta3_branch = tree->GetBranch("Mmm_pt3_eta3");
  if (Mmm_pt3_eta3_branch) Mmm_pt3_eta3_branch->SetAddress(&Mmm_pt3_eta3_);

  tree->SetMakeClass(0);
}

void LepNtup::GetEntry(unsigned int idx) {
  // this only marks branches as not loaded, saving a lot of time
  index = idx;
  run_isLoaded = false;
  lumi_isLoaded = false;
  evt_isLoaded = false;
  is_ee_isLoaded = false;
  is_mm_isLoaded = false;
  tag_p4_isLoaded = false;
  p4_isLoaded = false;
  pass_ttHTightId_isLoaded = false;
  mll_isLoaded = false;
  Mee_pt0_eta0_isLoaded = false;
  Mee_pt1_eta0_isLoaded = false;
  Mee_pt2_eta0_isLoaded = false;
  Mee_pt3_eta0_isLoaded = false;
  Mee_pt0_eta1_isLoaded = false;
  Mee_pt1_eta1_isLoaded = false;
  Mee_pt2_eta1_isLoaded = false;
  Mee_pt3_eta1_isLoaded = false;
  Mee_pt0_eta2_isLoaded = false;
  Mee_pt1_eta2_isLoaded = false;
  Mee_pt2_eta2_isLoaded = false;
  Mee_pt3_eta2_isLoaded = false;
  Mee_pt0_eta3_isLoaded = false;
  Mee_pt1_eta3_isLoaded = false;
  Mee_pt2_eta3_isLoaded = false;
  Mee_pt3_eta3_isLoaded = false;
  Mmm_pt0_eta0_isLoaded = false;
  Mmm_pt1_eta0_isLoaded = false;
  Mmm_pt2_eta0_isLoaded = false;
  Mmm_pt3_eta0_isLoaded = false;
  Mmm_pt0_eta1_isLoaded = false;
  Mmm_pt1_eta1_isLoaded = false;
  Mmm_pt2_eta1_isLoaded = false;
  Mmm_pt3_eta1_isLoaded = false;
  Mmm_pt0_eta2_isLoaded = false;
  Mmm_pt1_eta2_isLoaded = false;
  Mmm_pt2_eta2_isLoaded = false;
  Mmm_pt3_eta2_isLoaded = false;
  Mmm_pt0_eta3_isLoaded = false;
  Mmm_pt1_eta3_isLoaded = false;
  Mmm_pt2_eta3_isLoaded = false;
  Mmm_pt3_eta3_isLoaded = false;
}

void LepNtup::LoadAllBranches() {
  // load all branches
  if (run_branch != 0) run();
  if (lumi_branch != 0) lumi();
  if (evt_branch != 0) evt();
  if (is_ee_branch != 0) is_ee();
  if (is_mm_branch != 0) is_mm();
  if (tag_p4_branch != 0) tag_p4();
  if (p4_branch != 0) p4();
  if (pass_ttHTightId_branch != 0) pass_ttHTightId();
  if (mll_branch != 0) mll();
  if (Mee_pt0_eta0_branch != 0) Mee_pt0_eta0();
  if (Mee_pt1_eta0_branch != 0) Mee_pt1_eta0();
  if (Mee_pt2_eta0_branch != 0) Mee_pt2_eta0();
  if (Mee_pt3_eta0_branch != 0) Mee_pt3_eta0();
  if (Mee_pt0_eta1_branch != 0) Mee_pt0_eta1();
  if (Mee_pt1_eta1_branch != 0) Mee_pt1_eta1();
  if (Mee_pt2_eta1_branch != 0) Mee_pt2_eta1();
  if (Mee_pt3_eta1_branch != 0) Mee_pt3_eta1();
  if (Mee_pt0_eta2_branch != 0) Mee_pt0_eta2();
  if (Mee_pt1_eta2_branch != 0) Mee_pt1_eta2();
  if (Mee_pt2_eta2_branch != 0) Mee_pt2_eta2();
  if (Mee_pt3_eta2_branch != 0) Mee_pt3_eta2();
  if (Mee_pt0_eta3_branch != 0) Mee_pt0_eta3();
  if (Mee_pt1_eta3_branch != 0) Mee_pt1_eta3();
  if (Mee_pt2_eta3_branch != 0) Mee_pt2_eta3();
  if (Mee_pt3_eta3_branch != 0) Mee_pt3_eta3();
  if (Mmm_pt0_eta0_branch != 0) Mmm_pt0_eta0();
  if (Mmm_pt1_eta0_branch != 0) Mmm_pt1_eta0();
  if (Mmm_pt2_eta0_branch != 0) Mmm_pt2_eta0();
  if (Mmm_pt3_eta0_branch != 0) Mmm_pt3_eta0();
  if (Mmm_pt0_eta1_branch != 0) Mmm_pt0_eta1();
  if (Mmm_pt1_eta1_branch != 0) Mmm_pt1_eta1();
  if (Mmm_pt2_eta1_branch != 0) Mmm_pt2_eta1();
  if (Mmm_pt3_eta1_branch != 0) Mmm_pt3_eta1();
  if (Mmm_pt0_eta2_branch != 0) Mmm_pt0_eta2();
  if (Mmm_pt1_eta2_branch != 0) Mmm_pt1_eta2();
  if (Mmm_pt2_eta2_branch != 0) Mmm_pt2_eta2();
  if (Mmm_pt3_eta2_branch != 0) Mmm_pt3_eta2();
  if (Mmm_pt0_eta3_branch != 0) Mmm_pt0_eta3();
  if (Mmm_pt1_eta3_branch != 0) Mmm_pt1_eta3();
  if (Mmm_pt2_eta3_branch != 0) Mmm_pt2_eta3();
  if (Mmm_pt3_eta3_branch != 0) Mmm_pt3_eta3();
}

const int &LepNtup::run() {
  if (not run_isLoaded) {
    if (run_branch != 0) {
      run_branch->GetEntry(index);
    } else {
      printf("branch run_branch does not exist!\n");
      exit(1);
    }
    run_isLoaded = true;
  }
  return run_;
}

const int &LepNtup::lumi() {
  if (not lumi_isLoaded) {
    if (lumi_branch != 0) {
      lumi_branch->GetEntry(index);
    } else {
      printf("branch lumi_branch does not exist!\n");
      exit(1);
    }
    lumi_isLoaded = true;
  }
  return lumi_;
}

const unsigned long long &LepNtup::evt() {
  if (not evt_isLoaded) {
    if (evt_branch != 0) {
      evt_branch->GetEntry(index);
    } else {
      printf("branch evt_branch does not exist!\n");
      exit(1);
    }
    evt_isLoaded = true;
  }
  return evt_;
}

const int &LepNtup::is_ee() {
  if (not is_ee_isLoaded) {
    if (is_ee_branch != 0) {
      is_ee_branch->GetEntry(index);
    } else {
      printf("branch is_ee_branch does not exist!\n");
      exit(1);
    }
    is_ee_isLoaded = true;
  }
  return is_ee_;
}

const int &LepNtup::is_mm() {
  if (not is_mm_isLoaded) {
    if (is_mm_branch != 0) {
      is_mm_branch->GetEntry(index);
    } else {
      printf("branch is_mm_branch does not exist!\n");
      exit(1);
    }
    is_mm_isLoaded = true;
  }
  return is_mm_;
}

const ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > &LepNtup::tag_p4() {
  if (not tag_p4_isLoaded) {
    if (tag_p4_branch != 0) {
      tag_p4_branch->GetEntry(index);
    } else {
      printf("branch tag_p4_branch does not exist!\n");
      exit(1);
    }
    tag_p4_isLoaded = true;
  }
  return *tag_p4_;
}

const ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > &LepNtup::p4() {
  if (not p4_isLoaded) {
    if (p4_branch != 0) {
      p4_branch->GetEntry(index);
    } else {
      printf("branch p4_branch does not exist!\n");
      exit(1);
    }
    p4_isLoaded = true;
  }
  return *p4_;
}

const int &LepNtup::pass_ttHTightId() {
  if (not pass_ttHTightId_isLoaded) {
    if (pass_ttHTightId_branch != 0) {
      pass_ttHTightId_branch->GetEntry(index);
    } else {
      printf("branch pass_ttHTightId_branch does not exist!\n");
      exit(1);
    }
    pass_ttHTightId_isLoaded = true;
  }
  return pass_ttHTightId_;
}

const float &LepNtup::mll() {
  if (not mll_isLoaded) {
    if (mll_branch != 0) {
      mll_branch->GetEntry(index);
    } else {
      printf("branch mll_branch does not exist!\n");
      exit(1);
    }
    mll_isLoaded = true;
  }
  return mll_;
}

const vector<float> &LepNtup::Mee_pt0_eta0() {
  if (not Mee_pt0_eta0_isLoaded) {
    if (Mee_pt0_eta0_branch != 0) {
      Mee_pt0_eta0_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt0_eta0_branch does not exist!\n");
      exit(1);
    }
    Mee_pt0_eta0_isLoaded = true;
  }
  return *Mee_pt0_eta0_;
}

const vector<float> &LepNtup::Mee_pt1_eta0() {
  if (not Mee_pt1_eta0_isLoaded) {
    if (Mee_pt1_eta0_branch != 0) {
      Mee_pt1_eta0_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt1_eta0_branch does not exist!\n");
      exit(1);
    }
    Mee_pt1_eta0_isLoaded = true;
  }
  return *Mee_pt1_eta0_;
}

const vector<float> &LepNtup::Mee_pt2_eta0() {
  if (not Mee_pt2_eta0_isLoaded) {
    if (Mee_pt2_eta0_branch != 0) {
      Mee_pt2_eta0_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt2_eta0_branch does not exist!\n");
      exit(1);
    }
    Mee_pt2_eta0_isLoaded = true;
  }
  return *Mee_pt2_eta0_;
}

const vector<float> &LepNtup::Mee_pt3_eta0() {
  if (not Mee_pt3_eta0_isLoaded) {
    if (Mee_pt3_eta0_branch != 0) {
      Mee_pt3_eta0_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt3_eta0_branch does not exist!\n");
      exit(1);
    }
    Mee_pt3_eta0_isLoaded = true;
  }
  return *Mee_pt3_eta0_;
}

const vector<float> &LepNtup::Mee_pt0_eta1() {
  if (not Mee_pt0_eta1_isLoaded) {
    if (Mee_pt0_eta1_branch != 0) {
      Mee_pt0_eta1_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt0_eta1_branch does not exist!\n");
      exit(1);
    }
    Mee_pt0_eta1_isLoaded = true;
  }
  return *Mee_pt0_eta1_;
}

const vector<float> &LepNtup::Mee_pt1_eta1() {
  if (not Mee_pt1_eta1_isLoaded) {
    if (Mee_pt1_eta1_branch != 0) {
      Mee_pt1_eta1_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt1_eta1_branch does not exist!\n");
      exit(1);
    }
    Mee_pt1_eta1_isLoaded = true;
  }
  return *Mee_pt1_eta1_;
}

const vector<float> &LepNtup::Mee_pt2_eta1() {
  if (not Mee_pt2_eta1_isLoaded) {
    if (Mee_pt2_eta1_branch != 0) {
      Mee_pt2_eta1_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt2_eta1_branch does not exist!\n");
      exit(1);
    }
    Mee_pt2_eta1_isLoaded = true;
  }
  return *Mee_pt2_eta1_;
}

const vector<float> &LepNtup::Mee_pt3_eta1() {
  if (not Mee_pt3_eta1_isLoaded) {
    if (Mee_pt3_eta1_branch != 0) {
      Mee_pt3_eta1_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt3_eta1_branch does not exist!\n");
      exit(1);
    }
    Mee_pt3_eta1_isLoaded = true;
  }
  return *Mee_pt3_eta1_;
}

const vector<float> &LepNtup::Mee_pt0_eta2() {
  if (not Mee_pt0_eta2_isLoaded) {
    if (Mee_pt0_eta2_branch != 0) {
      Mee_pt0_eta2_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt0_eta2_branch does not exist!\n");
      exit(1);
    }
    Mee_pt0_eta2_isLoaded = true;
  }
  return *Mee_pt0_eta2_;
}

const vector<float> &LepNtup::Mee_pt1_eta2() {
  if (not Mee_pt1_eta2_isLoaded) {
    if (Mee_pt1_eta2_branch != 0) {
      Mee_pt1_eta2_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt1_eta2_branch does not exist!\n");
      exit(1);
    }
    Mee_pt1_eta2_isLoaded = true;
  }
  return *Mee_pt1_eta2_;
}

const vector<float> &LepNtup::Mee_pt2_eta2() {
  if (not Mee_pt2_eta2_isLoaded) {
    if (Mee_pt2_eta2_branch != 0) {
      Mee_pt2_eta2_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt2_eta2_branch does not exist!\n");
      exit(1);
    }
    Mee_pt2_eta2_isLoaded = true;
  }
  return *Mee_pt2_eta2_;
}

const vector<float> &LepNtup::Mee_pt3_eta2() {
  if (not Mee_pt3_eta2_isLoaded) {
    if (Mee_pt3_eta2_branch != 0) {
      Mee_pt3_eta2_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt3_eta2_branch does not exist!\n");
      exit(1);
    }
    Mee_pt3_eta2_isLoaded = true;
  }
  return *Mee_pt3_eta2_;
}

const vector<float> &LepNtup::Mee_pt0_eta3() {
  if (not Mee_pt0_eta3_isLoaded) {
    if (Mee_pt0_eta3_branch != 0) {
      Mee_pt0_eta3_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt0_eta3_branch does not exist!\n");
      exit(1);
    }
    Mee_pt0_eta3_isLoaded = true;
  }
  return *Mee_pt0_eta3_;
}

const vector<float> &LepNtup::Mee_pt1_eta3() {
  if (not Mee_pt1_eta3_isLoaded) {
    if (Mee_pt1_eta3_branch != 0) {
      Mee_pt1_eta3_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt1_eta3_branch does not exist!\n");
      exit(1);
    }
    Mee_pt1_eta3_isLoaded = true;
  }
  return *Mee_pt1_eta3_;
}

const vector<float> &LepNtup::Mee_pt2_eta3() {
  if (not Mee_pt2_eta3_isLoaded) {
    if (Mee_pt2_eta3_branch != 0) {
      Mee_pt2_eta3_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt2_eta3_branch does not exist!\n");
      exit(1);
    }
    Mee_pt2_eta3_isLoaded = true;
  }
  return *Mee_pt2_eta3_;
}

const vector<float> &LepNtup::Mee_pt3_eta3() {
  if (not Mee_pt3_eta3_isLoaded) {
    if (Mee_pt3_eta3_branch != 0) {
      Mee_pt3_eta3_branch->GetEntry(index);
    } else {
      printf("branch Mee_pt3_eta3_branch does not exist!\n");
      exit(1);
    }
    Mee_pt3_eta3_isLoaded = true;
  }
  return *Mee_pt3_eta3_;
}

const vector<float> &LepNtup::Mmm_pt0_eta0() {
  if (not Mmm_pt0_eta0_isLoaded) {
    if (Mmm_pt0_eta0_branch != 0) {
      Mmm_pt0_eta0_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt0_eta0_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt0_eta0_isLoaded = true;
  }
  return *Mmm_pt0_eta0_;
}

const vector<float> &LepNtup::Mmm_pt1_eta0() {
  if (not Mmm_pt1_eta0_isLoaded) {
    if (Mmm_pt1_eta0_branch != 0) {
      Mmm_pt1_eta0_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt1_eta0_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt1_eta0_isLoaded = true;
  }
  return *Mmm_pt1_eta0_;
}

const vector<float> &LepNtup::Mmm_pt2_eta0() {
  if (not Mmm_pt2_eta0_isLoaded) {
    if (Mmm_pt2_eta0_branch != 0) {
      Mmm_pt2_eta0_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt2_eta0_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt2_eta0_isLoaded = true;
  }
  return *Mmm_pt2_eta0_;
}

const vector<float> &LepNtup::Mmm_pt3_eta0() {
  if (not Mmm_pt3_eta0_isLoaded) {
    if (Mmm_pt3_eta0_branch != 0) {
      Mmm_pt3_eta0_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt3_eta0_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt3_eta0_isLoaded = true;
  }
  return *Mmm_pt3_eta0_;
}

const vector<float> &LepNtup::Mmm_pt0_eta1() {
  if (not Mmm_pt0_eta1_isLoaded) {
    if (Mmm_pt0_eta1_branch != 0) {
      Mmm_pt0_eta1_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt0_eta1_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt0_eta1_isLoaded = true;
  }
  return *Mmm_pt0_eta1_;
}

const vector<float> &LepNtup::Mmm_pt1_eta1() {
  if (not Mmm_pt1_eta1_isLoaded) {
    if (Mmm_pt1_eta1_branch != 0) {
      Mmm_pt1_eta1_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt1_eta1_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt1_eta1_isLoaded = true;
  }
  return *Mmm_pt1_eta1_;
}

const vector<float> &LepNtup::Mmm_pt2_eta1() {
  if (not Mmm_pt2_eta1_isLoaded) {
    if (Mmm_pt2_eta1_branch != 0) {
      Mmm_pt2_eta1_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt2_eta1_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt2_eta1_isLoaded = true;
  }
  return *Mmm_pt2_eta1_;
}

const vector<float> &LepNtup::Mmm_pt3_eta1() {
  if (not Mmm_pt3_eta1_isLoaded) {
    if (Mmm_pt3_eta1_branch != 0) {
      Mmm_pt3_eta1_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt3_eta1_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt3_eta1_isLoaded = true;
  }
  return *Mmm_pt3_eta1_;
}

const vector<float> &LepNtup::Mmm_pt0_eta2() {
  if (not Mmm_pt0_eta2_isLoaded) {
    if (Mmm_pt0_eta2_branch != 0) {
      Mmm_pt0_eta2_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt0_eta2_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt0_eta2_isLoaded = true;
  }
  return *Mmm_pt0_eta2_;
}

const vector<float> &LepNtup::Mmm_pt1_eta2() {
  if (not Mmm_pt1_eta2_isLoaded) {
    if (Mmm_pt1_eta2_branch != 0) {
      Mmm_pt1_eta2_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt1_eta2_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt1_eta2_isLoaded = true;
  }
  return *Mmm_pt1_eta2_;
}

const vector<float> &LepNtup::Mmm_pt2_eta2() {
  if (not Mmm_pt2_eta2_isLoaded) {
    if (Mmm_pt2_eta2_branch != 0) {
      Mmm_pt2_eta2_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt2_eta2_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt2_eta2_isLoaded = true;
  }
  return *Mmm_pt2_eta2_;
}

const vector<float> &LepNtup::Mmm_pt3_eta2() {
  if (not Mmm_pt3_eta2_isLoaded) {
    if (Mmm_pt3_eta2_branch != 0) {
      Mmm_pt3_eta2_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt3_eta2_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt3_eta2_isLoaded = true;
  }
  return *Mmm_pt3_eta2_;
}

const vector<float> &LepNtup::Mmm_pt0_eta3() {
  if (not Mmm_pt0_eta3_isLoaded) {
    if (Mmm_pt0_eta3_branch != 0) {
      Mmm_pt0_eta3_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt0_eta3_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt0_eta3_isLoaded = true;
  }
  return *Mmm_pt0_eta3_;
}

const vector<float> &LepNtup::Mmm_pt1_eta3() {
  if (not Mmm_pt1_eta3_isLoaded) {
    if (Mmm_pt1_eta3_branch != 0) {
      Mmm_pt1_eta3_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt1_eta3_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt1_eta3_isLoaded = true;
  }
  return *Mmm_pt1_eta3_;
}

const vector<float> &LepNtup::Mmm_pt2_eta3() {
  if (not Mmm_pt2_eta3_isLoaded) {
    if (Mmm_pt2_eta3_branch != 0) {
      Mmm_pt2_eta3_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt2_eta3_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt2_eta3_isLoaded = true;
  }
  return *Mmm_pt2_eta3_;
}

const vector<float> &LepNtup::Mmm_pt3_eta3() {
  if (not Mmm_pt3_eta3_isLoaded) {
    if (Mmm_pt3_eta3_branch != 0) {
      Mmm_pt3_eta3_branch->GetEntry(index);
    } else {
      printf("branch Mmm_pt3_eta3_branch does not exist!\n");
      exit(1);
    }
    Mmm_pt3_eta3_isLoaded = true;
  }
  return *Mmm_pt3_eta3_;
}


void LepNtup::progress( int nEventsTotal, int nEventsChain ){
  int period = 1000;
  if (nEventsTotal%1000 == 0) {
    // xterm magic from L. Vacavant and A. Cerri
    if (isatty(1)) {
      if ((nEventsChain - nEventsTotal) > period) {
        float frac = (float)nEventsTotal/(nEventsChain*0.01);
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
             "\033[0m\033[32m <---\033[0m\015", frac);
        fflush(stdout);
      }
      else {
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
               "\033[0m\033[32m <---\033[0m\015", 100.);
        cout << endl;
      }
    }
  }
}

namespace tas {

const int &run() { return lep.run(); }
const int &lumi() { return lep.lumi(); }
const unsigned long long &evt() { return lep.evt(); }
const int &is_ee() { return lep.is_ee(); }
const int &is_mm() { return lep.is_mm(); }
const ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > &tag_p4() { return lep.tag_p4(); }
const ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > &p4() { return lep.p4(); }
const int &pass_ttHTightId() { return lep.pass_ttHTightId(); }
const float &mll() { return lep.mll(); }
const vector<float> &Mee_pt0_eta0() { return lep.Mee_pt0_eta0(); }
const vector<float> &Mee_pt1_eta0() { return lep.Mee_pt1_eta0(); }
const vector<float> &Mee_pt2_eta0() { return lep.Mee_pt2_eta0(); }
const vector<float> &Mee_pt3_eta0() { return lep.Mee_pt3_eta0(); }
const vector<float> &Mee_pt0_eta1() { return lep.Mee_pt0_eta1(); }
const vector<float> &Mee_pt1_eta1() { return lep.Mee_pt1_eta1(); }
const vector<float> &Mee_pt2_eta1() { return lep.Mee_pt2_eta1(); }
const vector<float> &Mee_pt3_eta1() { return lep.Mee_pt3_eta1(); }
const vector<float> &Mee_pt0_eta2() { return lep.Mee_pt0_eta2(); }
const vector<float> &Mee_pt1_eta2() { return lep.Mee_pt1_eta2(); }
const vector<float> &Mee_pt2_eta2() { return lep.Mee_pt2_eta2(); }
const vector<float> &Mee_pt3_eta2() { return lep.Mee_pt3_eta2(); }
const vector<float> &Mee_pt0_eta3() { return lep.Mee_pt0_eta3(); }
const vector<float> &Mee_pt1_eta3() { return lep.Mee_pt1_eta3(); }
const vector<float> &Mee_pt2_eta3() { return lep.Mee_pt2_eta3(); }
const vector<float> &Mee_pt3_eta3() { return lep.Mee_pt3_eta3(); }
const vector<float> &Mmm_pt0_eta0() { return lep.Mmm_pt0_eta0(); }
const vector<float> &Mmm_pt1_eta0() { return lep.Mmm_pt1_eta0(); }
const vector<float> &Mmm_pt2_eta0() { return lep.Mmm_pt2_eta0(); }
const vector<float> &Mmm_pt3_eta0() { return lep.Mmm_pt3_eta0(); }
const vector<float> &Mmm_pt0_eta1() { return lep.Mmm_pt0_eta1(); }
const vector<float> &Mmm_pt1_eta1() { return lep.Mmm_pt1_eta1(); }
const vector<float> &Mmm_pt2_eta1() { return lep.Mmm_pt2_eta1(); }
const vector<float> &Mmm_pt3_eta1() { return lep.Mmm_pt3_eta1(); }
const vector<float> &Mmm_pt0_eta2() { return lep.Mmm_pt0_eta2(); }
const vector<float> &Mmm_pt1_eta2() { return lep.Mmm_pt1_eta2(); }
const vector<float> &Mmm_pt2_eta2() { return lep.Mmm_pt2_eta2(); }
const vector<float> &Mmm_pt3_eta2() { return lep.Mmm_pt3_eta2(); }
const vector<float> &Mmm_pt0_eta3() { return lep.Mmm_pt0_eta3(); }
const vector<float> &Mmm_pt1_eta3() { return lep.Mmm_pt1_eta3(); }
const vector<float> &Mmm_pt2_eta3() { return lep.Mmm_pt2_eta3(); }
const vector<float> &Mmm_pt3_eta3() { return lep.Mmm_pt3_eta3(); }

}

#include "rooutil.cc"
