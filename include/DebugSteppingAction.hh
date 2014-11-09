#ifndef DEBUGSTEPPINGACTION_HH
#define DEBUGSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"
#include "G4FieldManager.hh"
#include "G4PathFinder.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"

class DebugSteppingAction : public G4UserSteppingAction
{
public:
    DebugSteppingAction() {}
    virtual ~DebugSteppingAction() {}

    virtual void UserSteppingAction(const G4Step* aStep)
    {
        if (fpSteppingManager->GetfCurrentVolume()->GetLogicalVolume()->GetName() == "MagnetLogic")
        {

            if (G4TransportationManager::GetTransportationManager()->GetPropagatorInField()->GetVerboseLevel() == 0)
            {
                G4cout << "Major PropagatorInField reports, sir!" << G4endl;
                G4TransportationManager::GetTransportationManager()->GetPropagatorInField()->SetVerboseLevel(10);
            }
        }
    }

};

#endif // DEBUGSTEPPINGACTION_HH
