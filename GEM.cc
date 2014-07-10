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
#include "GEMParallelWorld.hh"

int main(int argc,char** argv)
{

    // Set the custom seed for the random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4long seed = time(NULL);
    G4Random::setTheSeed(seed);

#ifndef G4MULTITHREADED
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
        G4ThreeVector magField(0,0,0);
        for (G4int i = 0; i < 10; i++)
        {
            for (G4int j = 0 ; j < 10; j++)
            {
                magField.setX((G4double)i*(1500/10)-750);
                magField.setY((G4double)j*(1500/10)-750);
                std::stringstream X;
                X << magField.getX();
                G4String strX = X.str();
                std::stringstream Y;
                Y << magField.getY();
                G4String strY = Y.str();
                UImanager->ApplyCommand("/GEM/setMagField "+strX+" "+strY+" 0");
                UImanager->ApplyCommand("/run/beamOn 1000");
            }
        }
    }

    delete runManager;
    return 0;
}


