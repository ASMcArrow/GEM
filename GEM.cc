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
#include "GEMEventAction.hh"
#include "GEMRunAction.hh"
#include "GEMParallelWorld.hh"
#include "GEMActionInitialization.hh"

int main(int argc,char** argv)
{
 //Test
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

    GEMDetectorConstruction* massWorld = new GEMDetectorConstruction;
    massWorld->RegisterParallelWorld(new GEMParallelWorld("GEMParallelWorld"));
    runManager->SetUserInitialization(massWorld);

    G4VModularPhysicsList* physicsList = new GEMPhysicsList;
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new GEMActionInitialization);

    runManager->Initialize();

#ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
#endif

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (argc!=1)
    {
    // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else
    {
    // interactive mode : define UI session
#ifdef G4UI_USE
       G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
       UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
       UImanager->ApplyCommand("/control/execute init.mac");
#endif
       ui->SessionStart();
       delete ui;
#endif
    }

#ifdef G4VIS_USE
    delete visManager;
#endif
    delete runManager;
    return 0;
}


