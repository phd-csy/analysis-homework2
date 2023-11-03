#include "IBDSD.hh"

#include "DetectorConstruction.hh"
#include "G4AnalysisManager.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4Gamma.hh"
#include "G4Neutron.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4Positron.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "IBDHit.hh"

IBDSD::IBDSD(const G4String& sdname, const G4String& hcName) :
    G4VSensitiveDetector(sdname) { collectionName.insert(hcName); }

IBDSD::~IBDSD() {}

void IBDSD::Initialize(G4HCofThisEvent* hcOfThisEvent) {

    hc = new IBDHC(SensitiveDetectorName, collectionName[0]);
    auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hcOfThisEvent->AddHitsCollection(hcID, hc);
    for (auto i = 0; i < 2; i++)
        hc->insert(new IBDHit());
}

G4bool IBDSD::ProcessHits(G4Step* step, G4TouchableHistory*) {

    auto analysisManager = G4AnalysisManager::Instance();

    auto ftrack = step->GetTrack();
    auto particleDefinition = ftrack->GetDefinition();

    if (particleDefinition == G4AntiNeutrinoE::Definition() &&
        step->IsFirstStepInVolume()) {

        auto pdgID = particleDefinition->GetPDGEncoding();
        auto energy = ftrack->GetVertexKineticEnergy();

        analysisManager->FillH1(1, energy);

        analysisManager->FillNtupleDColumn(4, energy);
        analysisManager->AddNtupleRow();
    }

    if (particleDefinition == G4Positron::Definition()) {
        if (step->IsFirstStepInVolume()) {
            auto eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
            auto pdgID = particleDefinition->GetPDGEncoding();
            auto energy = ftrack->GetVertexKineticEnergy();

            analysisManager->FillH1(0, eventID);
            analysisManager->FillH1(2, energy);

            analysisManager->FillNtupleDColumn(0, eventID);
            analysisManager->FillNtupleDColumn(5, energy);
            analysisManager->AddNtupleRow();
        }
        (*hc)[0]->AddEdep(step->GetTotalEnergyDeposit());
    }

    if (particleDefinition == G4Gamma::Definition() &&
        ftrack->GetCreatorProcess()->GetProcessName() == "nCapture" &&
        step->IsFirstStepInVolume()) {

        auto nTime = step->GetPreStepPoint()->GetGlobalTime();

        analysisManager->FillH1(3, nTime);

        analysisManager->FillNtupleDColumn(6, nTime);
        analysisManager->AddNtupleRow();
    }

    if (particleDefinition == G4Neutron::Definition()) {
        (*hc)[1]->AddEdep(step->GetTotalEnergyDeposit());
    }

    return true;
}