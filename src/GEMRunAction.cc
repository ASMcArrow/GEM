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

        Depth[i] = 0;
    }
}

GEMRunAction::~GEMRunAction()
{}

G4Run* GEMRunAction::GenerateRun()
{
    return new GEMRun(DepthDetectorName, ProfileDetectorName, 0);
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
    if(!IsMaster()) return;

    GEMRun *gemRun = (GEMRun*)aRun;

    G4int hitNum1 = gemRun->GetNumberOfHits("DepthDetector");
    for (G4int i = 0; i < hitNum1; i++)
    {
        GEMDetectorHit* hit = (GEMDetectorHit*)(gemRun->GetHit("DepthDetector", i));
        if(hit != NULL)
        {
            G4int j = hit->GetPos()[0];
            Depth[j] = Depth[j]+hit->GetEdep();
        }
    }

    G4int hitNum2 = gemRun->GetNumberOfHits("ProfileDetector");
    for (G4int i = 0; i < hitNum2; i++)
    {
        GEMDetectorHit* hit = (GEMDetectorHit*)(gemRun->GetHit("ProfileDetector", i));
        if(hit != NULL)
        {
            G4int j = hit->GetPos()[0];
            G4int k = hit->GetPos()[1];
            Cells[j][k] = Cells[j][k]+hit->GetEdep();
        }
    }

    std::ofstream depthFile("GEMDepth.txt");
    for (G4int i = 0; i < 100; i++)
        depthFile << i*22.0/100.0 << " " << Depth[i]/Depth[0] << "\n";

    std::ofstream profileFile("GEMProfile.txt");
    G4double horizontal[101], vertical[101];
    for (G4int i = 0; i < 100; i++)
    {
        profileFile << "\n";
        for (G4int j = 0; j < 100; j++)
        {
            if ((i == 100)||(j == 100))
            {
                profileFile << (G4double)(i*30.0)/100.0 << " " << (G4double)(j*30.0/100.0) << " 0 \n";
            }
            else
                profileFile << i*30.0/100.0 << " " << j*30.0/100.0 << " " << Cells[i][j] << "\n";

            if (i == 50)
                horizontal[j] = Cells[i][j];

            if (j == 50)
                vertical[i] = Cells[i][j];
        }
    }

    horizontal[100] = 0;
    vertical[100] = 0;

    std::ofstream profileFileH("GEMProfile_H.txt");
    std::ofstream profileFileV("GEMProfile_V.txt");

    for (G4int box = 0; box <= 100; box++)
    {
            profileFileH << (G4double)(box*30.0/100.0) << " " << horizontal[box] << "\n";
            profileFileV << (G4double)(box*30.0/100.0) << " " << vertical[box] << "\n";
    }
}
