#include "GEMRunAction.hh"
#include "GEMRun.hh"
#include "GEMDetectorHit.hh"

#include "G4RunManager.hh"
#include "GEMDetectorConstruction.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Run.hh"

#include <fstream>
#define _USE_MATH_DEFINES

GEMRunAction::GEMRunAction(const G4String detectorName) : G4UserRunAction()
{
    DetectorName = detectorName;

    for (int i = 0; i < 200; i++)
    {
        for (int j = 0; j < 200; j++)
            Cells[i][j] = 0;
    }
}

GEMRunAction::~GEMRunAction()
{}

G4Run* GEMRunAction::GenerateRun()
{
  return new GEMRun(DetectorName, 0);
}

void GEMRunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
}

void GEMRunAction::EndOfRunAction(const G4Run* aRun)
{
    if(!IsMaster()) return;

    GEMRun *GEMRun = (GEMRun*)aRun;
    for (int i = 0; i < 200; i++)
    {
        for (int j = 0; j < 200; j++)
            Cells[i][j] += GEMRun->GetCells()[i][j];
    }

    std::ofstream mapFile("GEMUrban15.txt");

    for (G4int xBox = 0; xBox <= 200; xBox++)
    {
        mapFile << "\n";

        // bloody gnuplot!
        for (G4int yBox = 0; yBox <= 200; yBox++)
        {
            if ((yBox == 200)||(xBox == 200))
            {
                mapFile << (G4double)(xBox*6.0)/200.0 << " " << (G4double)(yBox*6.0/200.0) << " 0 \n";
            }
            else
                mapFile << (G4double)(xBox*6.0)/200.0 << " " << (G4double)(yBox*6.0/200.0) << " " << Cells[yBox][xBox] << " \n";
        }
    }  
}
