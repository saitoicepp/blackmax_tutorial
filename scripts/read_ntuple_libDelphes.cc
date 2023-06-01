#include "classes/DelphesClasses.h"
#include "classes/DelphesClasses.cc"

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif

#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TH1D.h>

void read_ntuple_libDelphes(const char* filename="BlackMaxLHArecord.root"){
    gSystem->Load("libDelphes");

    auto f = TFile::Open(filename);
    auto t = f->Get<TTree>("Delphes");

    // Define a reader to access variables in ntuples
    // Enegy unit: GeV
    TTreeReader reader(t);

    // Truth Particles
    TTreeReaderArray<GenParticle> particle(reader, "Particle");

    // Truth Jet
    TTreeReaderArray<Jet> genJet(reader, "GenJet");

    // Reco Jet
    TTreeReaderArray<Jet> jet(reader, "Jet");

    // Reco MET
    TTreeReaderArray<MissingET> missingET(reader, "MissingET");

    // Reco Electron
    TTreeReaderArray<Electron> electron(reader, "Electron");

    // Reco Muon
    TTreeReaderArray<Muon> muon(reader, "Muon");

    //// Define histogram
    auto h_truthElectrons_pt = new TH1D("h_truthElectrons_pt", "", 100, 0, 10000);
    auto h_truthMuons_pt = new TH1D("h_truthMuons_pt", "", 100, 0, 10000);
    auto h_truthJets_pt = new TH1D("h_truthJets_pt", "", 100, 0, 10000);
    auto h_jets_pt = new TH1D("h_jets_pt", "", 100, 0, 10000);
    auto h_electrons_pt = new TH1D("h_electrons_pt", "", 100, 0, 10000);
    auto h_muons_pt = new TH1D("h_muons_pt", "", 100, 0, 10000);
    auto h_met = new TH1D("h_met", "", 100, 0, 10000);

    while(reader.Next()){
        // Read only 1000 events
        if(reader.GetCurrentEntry() > 1000) break;

        // Loop over truth (generated) particles
        for(auto p : particle){
            if(p.Status != 1) continue;

            // Filter electrons
            if(abs(p.PID) == 11){
                h_truthElectrons_pt->Fill(p.P4().Pt());
            }
            // Filter muons
            if(abs(p.PID) == 13){
                h_truthMuons_pt->Fill(p.P4().Pt());
            }
            // You can find pdgId at https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf
        }

        // Loop over truth jets
        for(auto p : genJet){
            h_truthJets_pt->Fill(p.P4().Pt());
        }

        // Loop over reco jets
        for(auto p : jet){
            h_jets_pt->Fill(p.P4().Pt());
        }

        // Loop over reco electrons
        for(auto p : electron){
            h_electrons_pt->Fill(p.P4().Pt());
        }

        // Loop over reco muons
        for(auto p : muon){
            h_muons_pt->Fill(p.P4().Pt());
        }

        // Reco MET
        h_met->Fill(missingET.At(0).P4().Pt());

    }

    // Draw histogram
    // h_genCharginos_pt->Draw();
    const char* output_rootfilename = "hist.root";
    auto f_out = TFile::Open(output_rootfilename, "RECREATE");

    h_truthElectrons_pt->Write();
    h_truthMuons_pt->Write();
    h_truthJets_pt->Write();
    h_jets_pt->Write();
    h_electrons_pt->Write();
    h_muons_pt->Write();
    h_met->Write();

   f_out->Close();
}
