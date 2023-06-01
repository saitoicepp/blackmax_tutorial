#!/usr/bin/env python3
import ROOT
import click

@click.command()
@click.option('inputname', '-i', type=str, default="BlackMaxLHArecord.root")
@click.option('outputname', '-o', type=str, default="hist.root")
def main(inputname, outputname):
    ROOT.gSystem.Load("libDelphes")

    f = ROOT.TFile.Open(inputname)
    t = f.Get("Delphes")

    # Define histogram
    h_truthElectrons_pt = ROOT.TH1D("h_truthElectrons_pt", "", 100, 0, 10000)
    h_truthMuons_pt = ROOT.TH1D("h_truthMuons_pt", "", 100, 0, 10000)
    h_truthJets_pt = ROOT.TH1D("h_truthJets_pt", "", 100, 0, 10000)
    h_jets_pt = ROOT.TH1D("h_jets_pt", "", 100, 0, 10000)
    h_electrons_pt = ROOT.TH1D("h_electrons_pt", "", 100, 0, 10000)
    h_muons_pt = ROOT.TH1D("h_muons_pt", "", 100, 0, 10000)
    h_met = ROOT.TH1D("h_met", "", 100, 0, 10000)

    for i_event in range(t.GetEntries()):
        t.GetEntry(i_event)

        # Read only 1000 events
        if i_event > 1000:
            break

        # Loop over truth (generated) particles
        for p in t.Particle:
            if p.Status != 1:
                continue

            # Filter electrons
            if abs(p.PID) == 11:
                h_truthElectrons_pt.Fill(p.P4().Pt())

            # Filter muons
            if abs(p.PID) == 13:
                h_truthMuons_pt.Fill(p.P4().Pt())

            # You can find pdgId at https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf

        # Loop over truth jets
        for p in t.GenJet:
            h_truthJets_pt.Fill(p.P4().Pt())

        # Loop over reco jets
        for p in t.Jet:
           h_jets_pt.Fill(p.P4().Pt())

        # Loop over reco electrons
        for p in t.Electron:
            h_electrons_pt.Fill(p.P4().Pt())

        # Loop over reco muons
        for p in t.Muon:
            h_muons_pt.Fill(p.P4().Pt())

        # Reco MET
        h_met.Fill(t.MissingET.At(0).P4().Pt())

    f_out = ROOT.TFile.Open(outputname, "RECREATE")

    h_truthElectrons_pt.Write()
    h_truthMuons_pt.Write()
    h_truthJets_pt.Write()
    h_jets_pt.Write()
    h_electrons_pt.Write()
    h_muons_pt.Write()
    h_met.Write()

    f_out.Close()

if __name__ == '__main__':
    main()
