#!/bin/env python

import ROOT as r
import plottery_wrapper as p
from errors import E
import array

def get_muon_eff_sf(pt, eta, year):
    f_dy = r.TFile("hists/{year}/dy.root".format(year=year))
    h_dy_pass = f_dy.Get("Mmm_pt{pt}_eta{eta}_pass__Mll".format(pt=pt, eta=eta))
    h_dy_fail = f_dy.Get("Mmm_pt{pt}_eta{eta}_fail__Mll".format(pt=pt, eta=eta))
    f_mudata = r.TFile("hists/{year}/mudata.root".format(year=year))
    h_mudata_pass = f_mudata.Get("Mmm_pt{pt}_eta{eta}_pass__Mll".format(pt=pt, eta=eta))
    h_mudata_fail = f_mudata.Get("Mmm_pt{pt}_eta{eta}_fail__Mll".format(pt=pt, eta=eta))
    mc_pass_err = r.Double()
    mc_pass = h_dy_pass.IntegralAndError(0, h_dy_pass.GetNbinsX()+1, mc_pass_err)
    mc_fail_err = r.Double()
    mc_fail = h_dy_fail.IntegralAndError(0, h_dy_fail.GetNbinsX()+1, mc_fail_err)
    dt_pass_err = r.Double()
    dt_pass = h_mudata_pass.IntegralAndError(0, h_mudata_pass.GetNbinsX()+1, dt_pass_err)
    dt_fail_err = r.Double()
    dt_fail = h_mudata_fail.IntegralAndError(0, h_mudata_fail.GetNbinsX()+1, dt_fail_err)
    mc_pass = E(mc_pass, mc_pass_err)
    mc_fail = E(mc_fail, mc_fail_err)
    dt_pass = E(dt_pass, dt_pass_err)
    dt_fail = E(dt_fail, dt_fail_err)
    mc_eff = mc_pass / (mc_fail+mc_pass)
    dt_eff = dt_pass / (dt_fail+dt_pass)
    eff_sf = dt_eff / mc_eff
    return mc_eff, dt_eff, eff_sf

def get_electron_eff_sf(pt, eta, year):
    f_dy = r.TFile("hists/{year}/dy.root".format(year=year))
    h_dy_pass = f_dy.Get("Mee_pt{pt}_eta{eta}_pass__Mll".format(pt=pt, eta=eta))
    h_dy_fail = f_dy.Get("Mee_pt{pt}_eta{eta}_fail__Mll".format(pt=pt, eta=eta))
    f_eldata = r.TFile("hists/{year}/eldata.root".format(year=year))
    h_eldata_pass = f_eldata.Get("Mee_pt{pt}_eta{eta}_pass__Mll".format(pt=pt, eta=eta))
    h_eldata_fail = f_eldata.Get("Mee_pt{pt}_eta{eta}_fail__Mll".format(pt=pt, eta=eta))
    mc_pass_err = r.Double()
    mc_pass = h_dy_pass.IntegralAndError(0, h_dy_pass.GetNbinsX()+1, mc_pass_err)
    mc_fail_err = r.Double()
    mc_fail = h_dy_fail.IntegralAndError(0, h_dy_fail.GetNbinsX()+1, mc_fail_err)
    dt_pass_err = r.Double()
    dt_pass = h_eldata_pass.IntegralAndError(0, h_eldata_pass.GetNbinsX()+1, dt_pass_err)
    dt_fail_err = r.Double()
    dt_fail = h_eldata_fail.IntegralAndError(0, h_eldata_fail.GetNbinsX()+1, dt_fail_err)
    mc_pass = E(mc_pass, mc_pass_err)
    mc_fail = E(mc_fail, mc_fail_err)
    dt_pass = E(dt_pass, dt_pass_err)
    dt_fail = E(dt_fail, dt_fail_err)
    mc_eff = mc_pass / (mc_fail+mc_pass)
    dt_eff = dt_pass / (dt_fail+dt_pass)
    eff_sf = dt_eff / mc_eff
    return mc_eff, dt_eff, eff_sf

def print_sf_function(sfmap, fcn_name):
    rstr = []
    rstr.append("float ttH::{fcn_name}(float eta, float pt, int year, bool isAPV, int syst)".format(fcn_name=fcn_name))
    rstr.append("{")
    for year in ["2016APV", "2016", "2017", "2018"]:
        if year == "2016APV":
            rstr.append("    if (year == 2016 and isAPV)")
            rstr.append("    {")
        elif year == "2016":
            rstr.append("    else if (year == 2016 and not isAPV)")
            rstr.append("    {")
        elif year == "2017":
            rstr.append("    else if (year == 2017)")
            rstr.append("    {")
        elif year == "2018":
            rstr.append("    else // if (year == 2018)")
            rstr.append("    {")
        for pt in [0, 1, 2, 3, 4]:
            if pt == 0:
                rstr.append("        if (pt >= 40 and pt < 45)")
                rstr.append("        {")
            elif pt == 1:
                rstr.append("        else if (pt >= 45 and pt < 55)")
                rstr.append("        {")
            elif pt == 2:
                rstr.append("        else if (pt >= 55 and pt < 80)")
                rstr.append("        {")
            elif pt == 3:
                rstr.append("        else if (pt >= 80)")
                rstr.append("        {")
            elif pt == 4:
                rstr.append("        else")
                rstr.append("        {")
            for eta in [0, 1, 2, 3, 4]:
                if eta == 0:
                    rstr.append("            if (fabs(eta) >= 0.0 and fabs(eta) < 0.9)")
                    rstr.append("            {")
                elif eta == 1:
                    rstr.append("            else if (fabs(eta) >= 0.9 and fabs(eta) < 1.2)")
                    rstr.append("            {")
                elif eta == 2:
                    rstr.append("            else if (fabs(eta) >= 1.2 and fabs(eta) < 2.1)")
                    rstr.append("            {")
                elif eta == 3:
                    rstr.append("            else if (fabs(eta) >= 2.1)")
                    rstr.append("            {")
                elif eta == 4:
                    rstr.append("            else")
                    rstr.append("            {")
                if pt == 4 or eta == 4:
                    rstr.append("                return {}; // No SF computed for this phase-space".format(1))
                    rstr.append("            }")
                else:
                    sf = sfmap[year][pt][eta][2].val
                    err = sfmap[year][pt][eta][2].err
                    rstr.append("                return (syst == 0 ? {} : (syst == 1 ? {} : {}));".format(sf, sf+err, sf-err))
                    rstr.append("            }")
            rstr.append("        }")
        rstr.append("    }")
    rstr.append("}")
    print "\n".join(rstr)

def make_th2(sfmap, year, flavor, idx):
    if flavor == "mu":
        xs = array.array('d', [0., 0.9, 1.2, 2.1, 2.4])
        ys = array.array('d', [40, 45, 55, 80, 120])
        if idx == 2:
            th2 = r.TH2D("musf_{year}".format(year=year), "Muon POG loose -> ttH tight ID SF for {year}".format(year=year), 4, xs, 4, ys)
        elif idx == 0:
            th2 = r.TH2D("mumceff_{year}".format(year=year), "Muon POG loose -> ttH tight ID SF for {year}".format(year=year), 4, xs, 4, ys)
        elif idx == 1:
            th2 = r.TH2D("mudataeff_{year}".format(year=year), "Muon POG loose -> ttH tight ID SF for {year}".format(year=year), 4, xs, 4, ys)
        for pt in [0, 1, 2, 3]:
            for eta in [0, 1, 2, 3]:
                th2.SetBinContent(eta+1, pt+1, sfmap[year][pt][eta][idx].val)
                th2.SetBinError(eta+1, pt+1, sfmap[year][pt][eta][idx].err)
    elif flavor == "el":
        xs = array.array('d', [0., 0.9, 1.2, 2.1, 2.5])
        ys = array.array('d', [40, 45, 55, 80, 120])
        if idx == 2:
            th2 = r.TH2D("elsf_{year}".format(year=year), "Elec POG MVA noIso Loose -> ttH tight ID SF for {year}".format(year=year), 4, xs, 4, ys)
        elif idx == 0:
            th2 = r.TH2D("elmceff_{year}".format(year=year), "Muon POG loose -> ttH tight ID SF for {year}".format(year=year), 4, xs, 4, ys)
        elif idx == 1:
            th2 = r.TH2D("eldataeff_{year}".format(year=year), "Muon POG loose -> ttH tight ID SF for {year}".format(year=year), 4, xs, 4, ys)
        for pt in [0, 1, 2, 3]:
            for eta in [0, 1, 2, 3]:
                th2.SetBinContent(eta+1, pt+1, sfmap[year][pt][eta][idx].val)
                th2.SetBinError(eta+1, pt+1, sfmap[year][pt][eta][idx].err)
    return th2

if __name__ == "__main__":
    muon_sfs = {}
    for year in ["2016APV", "2016", "2017", "2018"]:
        muon_sfs[year] = {}
        for pt in [0, 1, 2, 3]:
            muon_sfs[year][pt] = {}
            for eta in [0, 1, 2, 3]:
                muon_sfs[year][pt][eta] = get_muon_eff_sf(pt, eta, year)
    print_sf_function(muon_sfs, "getMuonTightSFRederived")

    electron_sfs = {}
    for year in ["2016APV", "2016", "2017", "2018"]:
        electron_sfs[year] = {}
        for pt in [0, 1, 2, 3]:
            electron_sfs[year][pt] = {}
            for eta in [0, 1, 2, 3]:
                electron_sfs[year][pt][eta] = get_electron_eff_sf(pt, eta, year)
    print_sf_function(electron_sfs, "getElecTightSFRederived")

    f = r.TFile("TTHIDULSF.root", "recreate")
    for year in ["2016", "2016APV", "2017", "2018"]:
        for idx in [0, 1, 2]:
            make_th2(muon_sfs     , year , "mu" , idx).Write()
            make_th2(electron_sfs , year , "el" , idx).Write()
    f.Close()
