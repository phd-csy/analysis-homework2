#include "IBDSD.hh"

#include "DetectorConstruction.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4RunManager.hh"
#include "IBDHit.hh"

IBDSD::IBDSD(const G4String& sdname, const G4String& hcName) :
    G4VSensitiveDetector(sdname) { collectionName.insert(hcName); }

IBDSD::~IBDSD() {}

void IBDSD::Initialize(G4HCofThisEvent* hcOfThisEvent) {

    hc = new IBDHC(SensitiveDetectorName, collectionName[0]);
    auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hcOfThisEvent->AddHitsCollection(hcID, hc);
}

G4bool IBDSD::ProcessHits(G4Step* step, G4TouchableHistory*) {

    auto particleDefinition = step->GetTrack()->GetDefinition();
    auto hit = new IBDHit;
    auto globalTime = step->GetPostStepPoint()->GetGlobalTime();
    hit->SetGlobalTime(globalTime);
    hc->insert(hit);

    return true;
}