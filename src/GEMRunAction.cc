#include "GEMRunAction.hh"
#include "GEMRun.hh"
#include "GEMDetectorHit.hh"
#include "GEMDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "GEMDetectorConstruction.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Run.hh"

#include <fstream>
#define _USE_MATH_DEFINES

GEMRunAction::GEMRunAction(const G4String detectorName1, const G4String detectorName2) : G4UserRunAction()
{
    DepthDetectorName = detectorName1;
    ProfileDetectorName = detectorName2;
    ScanVertical = 0;
    ScanHorizontal = 0;

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
            Cells[i][j] = 0;
    }
}

GEMRunAction::~GEMRunAction()
{}

G4Run* GEMRunAction::GenerateRun()
{
    return new GEMRun(DepthDetectorName, ProfileDetectorName, 1);
}

void GEMRunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

    G4ThreeVector magField(0,0,0);
    magField.setX((G4double)ScanHorizontal*(1500/10)-750);
    magField.setY((G4double)ScanVertical*(1500/10)-750);
    magField.setZ(0);
    ScanVertical++;
    if (ScanVertical > 10)
    {
        ScanVertical = 0;
        ScanHorizontal++;
    }

    GEMDetectorConstruction::MagneticField->SetFieldValue(magField*gauss);

#ifndef G4MULTITHREADED
    G4MTRunManager::GetMasterRunManager()->GeometryHasBeenModified();
#else
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
#endif
}

void GEMRunAction::EndOfRunAction(const G4Run* aRun)
{
    /*
     *     G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();*/

    if(!IsMaster()) return;

    GEMRun *gemRun = (GEMRun*)aRun;
    G4int hitNum = gemRun->GetNumberOfHits();
    for (G4int i = 0; i < hitNum; i++)
    {
        GEMDetectorHit* hit = (GEMDetectorHit*)(gemRun->GetHit(i));
        if(hit != NULL)
        {
            /*  G4double i = ((*CHC)[h])->GetZID();
            G4int roundi = floor(i + 0.5);
            m_Bins[roundi+300] = m_Bins[roundi+300]+((*CHC)[h])->GetEdep(); */

            G4int j = hit->GetPos()[0];
            G4int k = hit->GetPos()[1];
            Cells[j][k] = Cells[j][k]+hit->GetEdep();
        }
    }

    /*std::stringstream ss;
    ss << ScanHorizontal;
    G4String name("GEMTest_"+ss.str()+".txt");
    std::ofstream File(name);
    for (G4int i = 0; i < 100; i++)
        File << i*22.0/100.0 << " " << Cells[i] << "\n";

    for (int i = 0; i < 100; i++)
    {
        Cells[i] = 0;
    }*/

    std::ofstream mapFile("GEMVox.txt");
     for (G4int i = 0; i < 100; i++)
     {
          for (G4int j = 0; j < 100; j++)
              mapFile << i*30.0/100.0 << " " << j*30.0/100.0 << " " << Cells[i][j] << "\n";

     }

}
