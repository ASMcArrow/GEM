#include "GEMRunAction.hh"
#include "GEMActionInitialization.hh"
#include "GEMPrimaryGeneratorAction.hh"
#include "DebugTrackingAction.hh"
#include "DebugEventAction.hh"

GEMActionInitialization::GEMActionInitialization()
    : G4VUserActionInitialization()
{}

GEMActionInitialization::~GEMActionInitialization()
{}

void GEMActionInitialization::BuildForMaster() const
{
    SetUserAction(new GEMRunAction(/*"DepthDetector"*/" ", "ProfileDetectorZero", "ProfileDetectorIso"));
}

void GEMActionInitialization::Build() const
{
    SetUserAction(new GEMPrimaryGeneratorAction);
    SetUserAction(new GEMRunAction(/*"DepthDetector"*/" ", "ProfileDetectorZero", "ProfileDetectorIso"));
    SetUserAction(new DebugTrackingAction());
    SetUserAction(new DebugEventAction());
}
