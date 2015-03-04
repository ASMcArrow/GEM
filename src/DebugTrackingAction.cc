#include "DebugTrackingAction.hh"
#include "DebugTrajectory.hh"

#include "G4TrackingManager.hh"

void DebugTrackingAction::PreUserTrackingAction(const G4Track *aTrack)
{
  //  DebugTrajectory *aTrajectory = new DebugTrajectory();
   // this->fpTrackingManager->SetTrajectory(aTrajectory);
   // G4cout << "PreUserTrackingAction: verbosity " << Verbosity << G4endl;
   // this->fpTrackingManager->SetVerboseLevel(Verbosity);
   // this->fpTrackingManager->SetStoreTrajectory(SaveTrajectory);
}
