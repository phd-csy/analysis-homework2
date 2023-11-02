#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4HEPEvtInterface.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction() :
    G4VUserPrimaryGeneratorAction() {
    const char* filename = "IBD_OSC_noME.data";
    HEPEvt = new G4HEPEvtInterface(filename);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete HEPEvt;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    HEPEvt->GeneratePrimaryVertex(anEvent);
}