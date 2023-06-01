#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TH1D.h>

const float ELECTRON_MASS = 0.0005109;
const float MUON_MASS = 0.1056599;

void read_ntuple(const char* filename="100000_10000_10000_1_6_CTEQ6L1.root"){
    auto f = TFile::Open(filename);
    auto t = f->Get<TTree>("Delphes");

    // Define a reader to access variables in ntuples
    // Enegy unit: GeV
    TTreeReader reader(t);

    // Truth Particles
    TTreeReaderArray<float> particle_PT(reader, "Particle.PT");
    TTreeReaderArray<float> particle_Eta(reader, "Particle.Eta");
    TTreeReaderArray<float> particle_Phi(reader, "Particle.Phi");
    TTreeReaderArray<float> particle_Mass(reader, "Particle.Mass");
    TTreeReaderArray<int> particle_PID(reader, "Particle.PID");
    TTreeReaderArray<int> particle_Status(reader, "Particle.Status");

    // Truth Jet
    TTreeReaderArray<float> genJet_PT(reader, "GenJet.PT");
    TTreeReaderArray<float> genJet_Eta(reader, "GenJet.Eta");
    TTreeReaderArray<float> genJet_Phi(reader, "GenJet.Phi");
    TTreeReaderArray<float> genJet_Mass(reader, "GenJet.Mass");

    // Reco Jet
    TTreeReaderArray<float> jet_PT(reader, "Jet.PT");
    TTreeReaderArray<float> jet_Eta(reader, "Jet.Eta");
    TTreeReaderArray<float> jet_Phi(reader, "Jet.Phi");
    TTreeReaderArray<float> jet_Mass(reader, "Jet.Mass");

    // Reco MET
    TTreeReaderArray<float> missingET_MET(reader, "MissingET.MET");
    TTreeReaderArray<float> missingET_Eta(reader, "MissingET.Eta");
    TTreeReaderArray<float> missingET_Phi(reader, "MissingET.Phi");

    // Reco Electron
    TTreeReaderArray<float> electron_PT(reader, "Electron.PT");
    TTreeReaderArray<float> electron_Eta(reader, "Electron.Eta");
    TTreeReaderArray<float> electron_Phi(reader, "Electron.Phi");

    // Reco Muon
    TTreeReaderArray<float> muon_PT(reader, "Muon.PT");
    TTreeReaderArray<float> muon_Eta(reader, "Muon.Eta");
    TTreeReaderArray<float> muon_Phi(reader, "Muon.Phi");

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
        int n_particle = particle_PT.GetSize();
        for(int i=0; i<n_particle; i++){
            if(particle_Status.At(i) != 1) continue;

            ROOT::Math::PtEtaPhiMVector p4(particle_PT.At(i), particle_Eta.At(i), particle_Phi.At(i), particle_Mass.At(i));

            // Filter electrons
            if(abs(particle_PID.At(i)) == 11){
                h_truthElectrons_pt->Fill(p4.Pt());
            }
            // Filter muons
            if(abs(particle_PID.At(i)) == 13){
                h_truthMuons_pt->Fill(p4.Pt());
            }
            // You can find pdgId at https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf
        }

        // Loop over truth jets
        int n_genJet = genJet_PT.GetSize();
        for(int i=0; i<n_genJet; i++){
            ROOT::Math::PtEtaPhiMVector p4(genJet_PT.At(i), genJet_Eta.At(i), genJet_Phi.At(i), genJet_Mass.At(i));

            h_truthJets_pt->Fill(p4.Pt());
        }

        // Loop over reco jets
        int n_jet = jet_PT.GetSize();
        for(int i=0; i<n_jet; i++){
            ROOT::Math::PtEtaPhiMVector p4(jet_PT.At(i), jet_Eta.At(i), jet_Phi.At(i), jet_Mass.At(i));

            h_jets_pt->Fill(p4.Pt());
        }

        // Loop over reco electrons
        int n_electron = electron_PT.GetSize();
        for(int i=0; i<n_electron; i++){
            ROOT::Math::PtEtaPhiMVector p4(electron_PT.At(i), electron_Eta.At(i), electron_Phi.At(i), ELECTRON_MASS);

            h_electrons_pt->Fill(p4.Pt());
        }

        // Loop over reco muons
        int n_muon = muon_PT.GetSize();
        for(int i=0; i<n_muon; i++){
            ROOT::Math::PtEtaPhiMVector p4(muon_PT.At(i), muon_Eta.At(i), muon_Phi.At(i), MUON_MASS);

            h_muons_pt->Fill(p4.Pt());
        }

        // Reco MET
        ROOT::Math::PtEtaPhiMVector met(missingET_MET.At(0), missingET_Eta.At(0), missingET_Phi.At(0), 0.);

        h_met->Fill(met.Pt());

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
