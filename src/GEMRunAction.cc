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

GEMRunAction::GEMRunAction(const G4String detectorName) : G4UserRunAction()
{
    DetectorName = detectorName;
    scan = 0;

    for (int i = 0; i < 100; i++)
    {
        Cells[i] = 0;
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

    G4ThreeVector magField(0,0,0);
    magField.setX((G4double)scan*(1500/10)-750);
    magField.setY((G4double)scan*(1500/10)-750);
    magField.setZ(0);
    scan++;
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
            Cells[j] = Cells[j]+hit->GetEdep();
        }
    }

    std::stringstream ss;
    ss << scan;
    G4String name("GEMTest_"+ss.str()+".txt");
    std::ofstream File(name);
    for (G4int i = 0; i < 100; i++)
        File << i*22.0/100.0 << " " << Cells[i] << "\n";

    for (int i = 0; i < 100; i++)
    {
        Cells[i] = 0;
    }
}
