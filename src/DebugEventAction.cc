#include "DebugEventAction.hh"
#include "DebugTrackingAction.hh"

#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"

G4ThreadLocal int DebugEventAction::RunCount = 0;

DebugEventAction::DebugEventAction()
{}

void DebugEventAction::BeginOfEventAction(const G4Event *anEvent)
{}


void DebugEventAction::EndOfEventAction(const G4Event *anEvent)
{
    if (anEvent->GetEventID() > 10)
        ((DebugTrackingAction*)fpEventManager->GetTrackingManager()->GetUserTrackingAction())->ShutUp();
}
