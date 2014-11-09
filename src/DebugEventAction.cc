#include "DebugEventAction.hh"
#include "DebugTrackingAction.hh"

#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"

G4ThreadLocal int DebugEventAction::EventCount = 0;

DebugEventAction::DebugEventAction()
{}

void DebugEventAction::BeginOfEventAction(const G4Event *anEvent)
{
    if ((anEvent->GetEventID() == 500000)||(anEvent->GetEventID() == 1000000))
        G4cout << "This is new event #" << anEvent->GetEventID() << G4endl;
}

void DebugEventAction::EndOfEventAction(const G4Event *anEvent)
{

}
