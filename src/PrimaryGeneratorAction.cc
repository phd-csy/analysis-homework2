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

    G4double radius = 18.5 * m;
    G4double x = 0., y = 0., z = 0.;
    G4ThreeVector vertex;

    x = radius * 2.;
    y = radius * 2.;
    z = radius * 2.;

    while (((x * x) + (y * y) + (z * z)) > (radius * radius)) {
        x = G4UniformRand();
        y = G4UniformRand();
        z = G4UniformRand();

        x = (x * 2. * radius) - radius;
        y = (y * 2. * radius) - radius;
        z = (z * 2. * radius) - radius;
    }

    vertex.setX(x);
    vertex.setY(y);
    vertex.setZ(z);

    HEPEvt->SetParticlePosition(vertex);

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH3(0, x, y, z);
    analysisManager->FillNtupleDColumn(1, x);
    analysisManager->FillNtupleDColumn(2, y);
    analysisManager->FillNtupleDColumn(3, z);
    analysisManager->AddNtupleRow();

    HEPEvt->GeneratePrimaryVertex(anEvent);
}