#include "RunAction.hh"

#include "DetectorConstruction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
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
