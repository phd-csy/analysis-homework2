#ifndef IBDHIT_HH
#define IBDHIT_HH

#include "G4THitsCollection.hh"
#include "G4VHit.hh"

class IBDHit : public G4VHit {
public:
    void SetPosition(G4double pos) { position = pos; }
    G4double GetPosition() const { return position; }

    void SetGlobalTime(G4double time) { globalTime = time; }
    G4double GetGlobalTime() const { return globalTime; }


private:
    G4double position;
    G4double globalTime;
};

using IBDHC = G4THitsCollection<IBDHit>;

#endif