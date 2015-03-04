
//#define G4MULTITHREADED
#define G4DEBUG_FIELD

#undef G4UI_USE
#undef G4VIS_USE
#undef G4MULTITHREADED

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
#include "G4Navigator.hh"
#include "G4GeometryManager.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"
#include "G4PathFinder.hh"
#include "G4FPEDetection.hh"
#include "G4RunManagerKernel.hh"
#include "G4Threading.hh"
#include "G4WorkerRunManager.hh"
#include "G4EventManager.hh"
#include "G4EmParameters.hh"

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

    //    G4cout << "Computed tolerance = "
    //           << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
    //           << " mm" << G4endl;

    GEMDetectorConstruction* massWorld = new GEMDetectorConstruction;
    massWorld->RegisterParallelWorld(new GEMParallelWorld("GEMParallelWorld"));
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
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

#ifndef G4UI_USE
    for (G4int i = 0 ; i < 100; i++)
    {
        UImanager->ApplyCommand("/run/geometryModified");
        runManager->BeamOn(1000000);
    }
#endif

#ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->SetVerboseLevel(4);
    visManager->Initialize();
#endif

#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac");
#endif
    ui->SessionStart();
    delete ui;
#endif

    delete runManager;
    return 0;
}


