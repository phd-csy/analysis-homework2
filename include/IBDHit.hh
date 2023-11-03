#ifndef IBDHIT_HH
#define IBDHIT_HH

#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"

class IBDHit : public G4VHit {
public:
    void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    G4ThreeVector GetPos() const { return fPos; }

    void SetTime(G4double time) { fTime = time; }
    G4double GetTime() const { return fTime; }

    void SetEdep(G4double de) { fEdep = de; }
    void AddEdep(G4double de) { fEdep += de; }
    G4double GetEdep() const { return fEdep; }

    void SetID(G4int id) { fID = id; }
    G4int GetID() const { return fID; }

private:
    G4ThreeVector fPos;
    G4double fTime;
    G4double fEdep = 0.;
    G4int fID;
};

using IBDHC = G4THitsCollection<IBDHit>;

#endif