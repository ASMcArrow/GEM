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

    static G4ThreadLocal G4int EventCount;

private:
    G4int i;
};

#endif // DEBUGEVENTACTION_HH
