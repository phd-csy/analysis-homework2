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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event) {

    auto ibdHCid = G4SDManager::GetSDMpointer()->GetCollectionID("IBDHitsCollection");
    auto ibdHC = static_cast<IBDHC*>(event->GetHCofThisEvent()->GetHC(ibdHCid));

    auto analysisManager = G4AnalysisManager::Instance();

    auto eventID = event->GetEventID();

    for (long unsigned int i = 0; i < ibdHC->entries(); i++) {

        auto ibdHit = (*ibdHC)[i];

        analysisManager->FillNtupleDColumn(0, eventID);
        analysisManager->AddNtupleRow();
    }

    auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    if ((printModulo > 0) && (eventID % printModulo == 0)) {
        G4cout << "---> End of event: " << eventID << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
