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
}

void GEMRunAction::EndOfRunAction(const G4Run* aRun)
{
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

     std::ofstream File("GEMScanningTest.txt");
     for (G4int i = 0; i < 100; i++)
         File << i*22.0/100.0 << " " << Cells[i]/Cells[0] << "\n";
}
