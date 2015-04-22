#include "GEMRunAction.hh"
#include "GEMActionInitialization.hh"
#include "GEMPrimaryGeneratorAction.hh"
#include "DebugTrackingAction.hh"
#include "DebugEventAction.hh"
#include "DebugSteppingAction.hh"

GEMActionInitialization::GEMActionInitialization()
    : G4VUserActionInitialization()
{
    NameVector.push_back("DepthDetector");
    NameVector.push_back("ProfileDetectorZero");
    NameVector.push_back("ProfileDetectorIso");
}

GEMActionInitialization::~GEMActionInitialization()
{}

void GEMActionInitialization::BuildForMaster() const
{
    SetUserAction(new GEMRunAction(NameVector));
}

void GEMActionInitialization::Build() const
{
    SetUserAction(new GEMPrimaryGeneratorAction);
    SetUserAction(new GEMRunAction(NameVector));
   // SetUserAction(new DebugSteppingAction());
   // SetUserAction(new DebugTrackingAction());
   // SetUserAction(new DebugEventAction());
}
