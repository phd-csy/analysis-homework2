#include "RunAction.hh"

#include "DetectorConstruction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Timer.hh"
#include "IBDHit.hh"
#include "IBDSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction() :
    G4UserRunAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {

    auto analysisManager = G4AnalysisManager::Instance();

#ifdef G4MULTITHREADED
    analysisManager->SetNtupleMerging(true);
#endif

    analysisManager->OpenFile("IBD.root");

    analysisManager->CreateH1("eventID", "eventID", 100, 0, 100000);
    analysisManager->CreateH1("nuEnergy", "nuEnergy", 500, 0, 15 * MeV);
    analysisManager->CreateH1("ePlusEnergy", "ePlusEnergy", 500, 0, 15 * MeV);
    analysisManager->CreateH1("captureTime", "captureTime", 100, 0, 200 * us);
    analysisManager->CreateH1("promptEdep", "promptEdep", 1000, 0, 10 * MeV);
    analysisManager->CreateH1("delayedEdep", "delayedEdep", 1000, 0, 200 * keV);
    analysisManager->CreateH1("promptEdep_smear", "promptEdep_smear", 1000, 0, 10 * MeV);
    analysisManager->CreateH1("delayedEdep_smear", "delayedEdep_smear", 1000, 0, 200 * keV);

    analysisManager->CreateH3("vertex", "vertex", 100, -20 * m, 20 * m, 100, -20 * m, 20 * m, 100, -20 * m, 20 * m);

    analysisManager->CreateNtuple("result", "IBD event");
    analysisManager->CreateNtupleDColumn("eventID");
    analysisManager->CreateNtupleDColumn("IBDVer_X");
    analysisManager->CreateNtupleDColumn("IBDVer_Y");
    analysisManager->CreateNtupleDColumn("IBDVer_Z");
    analysisManager->CreateNtupleDColumn("nuEnergy");
    analysisManager->CreateNtupleDColumn("ePlusEnergy");
    analysisManager->CreateNtupleDColumn("captureTime");
    analysisManager->CreateNtupleDColumn("promptEdep");
    analysisManager->CreateNtupleDColumn("delayedEdep");
    analysisManager->CreateNtupleDColumn("promptEdep_smeared");
    analysisManager->CreateNtupleDColumn("delayedEdep_smeared");

    analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*) {

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
    analysisManager->Clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
