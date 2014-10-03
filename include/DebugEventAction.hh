#ifndef DEBUGEVENTACTION_HH
#define DEBUGEVENTACTION_HH

#include "G4UserEventAction.hh"

#include "globals.hh"

class DebugEventAction : public G4UserEventAction
{

public:
    DebugEventAction();
    virtual ~DebugEventAction() {}

    void BeginOfEventAction(const G4Event* anEvent);
    void EndOfEventAction(const G4Event* anEvent);

    static G4ThreadLocal G4int RunCount;

private:
    G4int CollectionID1, CollectionID2, CollectionID3;
    G4int PreviousNHits, CurrentNHits;
};

#endif // DEBUGEVENTACTION_HH
