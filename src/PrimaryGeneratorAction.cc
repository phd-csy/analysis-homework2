#include "PrimaryGeneratorAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4HEPEvtInterface.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction() :
    G4VUserPrimaryGeneratorAction() {
    HEPEvt = new G4HEPEvtInterface("IBD_OSC_noME.data");
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete HEPEvt;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    auto radius = 18.5 * m;
    G4double theta = G4UniformRand() * 180.0 * deg; // 均匀分布的角度
    G4double phi = G4UniformRand() * 360.0 * deg;   // 均匀分布的角度
    G4double x0 = radius * sin(theta) * cos(phi);
    G4double y0 = radius * sin(theta) * sin(phi);
    G4double z0 = radius * cos(theta);

    auto vertex = G4ThreeVector(x0, y0, z0);

    HEPEvt->SetParticlePosition(vertex);

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(1, x0);
    analysisManager->FillNtupleDColumn(2, y0);
    analysisManager->FillNtupleDColumn(3, z0);

    HEPEvt->GeneratePrimaryVertex(anEvent);
}