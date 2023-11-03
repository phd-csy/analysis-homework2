#include "EventAction.hh"

#include "DetectorConstruction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Trajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "IBDHit.hh"
#include "IBDSD.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event) {

    auto ibdHCid = G4SDManager::GetSDMpointer()->GetCollectionID("IBDHitsCollection");
    auto ibdHC = static_cast<IBDHC*>(event->GetHCofThisEvent()->GetHC(ibdHCid));

    auto analysisManager = G4AnalysisManager::Instance();

    auto eventID = event->GetEventID();

    // for (long unsigned int i = 0; i < ibdHC->entries(); i++) {

    // auto ibdHit = (*ibdHC)[i];

    // analysisManager->FillNtupleDColumn(0, eventID);
    // analysisManager->AddNtupleRow();
    // }
    auto promptEdep = (*ibdHC)[0]->GetEdep();
    auto delayEdep = (*ibdHC)[1]->GetEdep();

    auto promptEdep_smear = G4RandGauss::shoot(promptEdep, 0.03 * promptEdep / 2.355);
    auto delayEdep_smear = G4RandGauss::shoot(delayEdep, 0.3 * delayEdep / 2.355);

    analysisManager->FillH1(4, promptEdep);
    analysisManager->FillH1(5, delayEdep);
    analysisManager->FillH1(6, promptEdep_smear);
    analysisManager->FillH1(7, delayEdep_smear);

    analysisManager->FillNtupleDColumn(7, promptEdep);
    analysisManager->FillNtupleDColumn(8, delayEdep);
    analysisManager->FillNtupleDColumn(9, promptEdep_smear);
    analysisManager->FillNtupleDColumn(10, delayEdep_smear);
    analysisManager->AddNtupleRow();

    auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    if ((printModulo > 0) && (eventID % printModulo == 0)) {
        G4cout << "---> End of event: " << eventID << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
