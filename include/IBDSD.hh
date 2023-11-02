#ifndef IBDSD_HH
#define IBDSD_HH

#include "G4OpticalPhoton.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "IBDHit.hh"

class IBDSD : public G4VSensitiveDetector {
public:
    IBDSD(const G4String&, const G4String&);
    ~IBDSD() override;

    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;

private:
    IBDHC* hc;
};

#endif