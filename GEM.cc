#include <cstdio>
#include <ctime>

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4UImanager.hh"
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "GEMPhysicsList.hh"
#include "GEMDetectorConstruction.hh"
#include "GEMPrimaryGeneratorAction.hh"
#include "GEMActionInitialization.hh"
#include "GEMVoxParallelWorld.hh"
#include "GEMParallelWorld.hh"
#include "G4Navigator.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"
#include "G4ExceptionHandler.hh"
#include "G4StateManager.hh"
#include "G4ParticleChange.hh"
#include "G4CoupledTransportation.hh"

int main(int argc,char** argv)
{
    // Set the custom seed for the random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4long seed = time(NULL);
    G4Random::setTheSeed(seed);

#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(8);
#else
    G4RunManager* runManager = new G4RunManager;
#endif

    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(4.0*m);

    G4cout << "Computed tolerance = "
           << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
           << " mm" << G4endl;

    GEMDetectorConstruction* massWorld = new GEMDetectorConstruction;
    //massWorld->RegisterParallelWorld(new GEMParallelWorld("GEMParallelWorld"));
    massWorld->RegisterParallelWorld(new GEMVoxParallelWorld("GEMVoxParallelWorld"));
    runManager->SetUserInitialization(massWorld);

    G4VModularPhysicsList* physicsList = new GEMPhysicsList;
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new GEMActionInitialization);

    runManager->Initialize();

    G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
    navigator->SetPushVerbosity(false);
    runManager->SetVerboseLevel(0);

    G4VExceptionHandler *handler = G4StateManager::GetStateManager()->GetExceptionHandler();

    for (G4int i = 0 ; i < 100; i++)
    {
        runManager->BeamOn(1000000);
    }

    /*#ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
#endif

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac");
#endif
    ui->SessionStart();
    delete ui;
#endif*/

    delete runManager;
    return 0;
}


