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

GEMRunAction::GEMRunAction(const G4String detectorName1, const G4String detectorName2, const G4String detectorName3) : G4UserRunAction()
{
    DepthDetectorName = detectorName1;
    ProfileDetectorName1 = detectorName2;
    ProfileDetectorName2 = detectorName3;
    ScanVertical = 0;
    ScanHorizontal = 0;

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            Cells1[i][j] = 0;
            Cells2[i][j] = 0;
        }

        Depth[i] = 0;
    }
}

GEMRunAction::~GEMRunAction()
{}

G4Run* GEMRunAction::GenerateRun()
{
    return new GEMRun(DepthDetectorName, ProfileDetectorName1, ProfileDetectorName2, 0);
}

void GEMRunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

    G4ThreeVector magField(0,0,0);
    magField.setX((G4double)ScanHorizontal*(1400/10)-700);
    magField.setY((G4double)ScanVertical*(1400/10)-700);
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

    std::ofstream depthFile("GEMDepth.txt");
    for (G4int i = 0; i < 100; i++)
        depthFile << i*22.0/100.0 << " " << Depth[i]/Depth[0] << "\n";

    this->DumpProfile("Zero", gemRun);
    this->DumpProfile("Peak", gemRun);
}

void GEMRunAction::DumpProfile(G4String type, GEMRun *gemRun)
{
    G4int hitNum;
    G4String detectorName;
    G4String fileName;

    if (type == "Zero")
    {
        hitNum = gemRun->GetNumberOfHits("ProfileDetectorZero");
        detectorName = "ProfileDetectorZero";
        fileName = "GEMProfileZero.txt";
    }
    else if (type == "Peak")
    {
        hitNum = gemRun->GetNumberOfHits("ProfileDetectorPeak");
        detectorName = "ProfileDetectorPeak";
        fileName = "GEMProfilePeak.txt";
    }

    for (G4int i = 0; i < hitNum; i++)
    {
        GEMDetectorHit* hit = (GEMDetectorHit*)(gemRun->GetHit(detectorName, i));

        if(hit != NULL)
        {
            G4int j = hit->GetPos()[0];
            G4int k = hit->GetPos()[1];
            if (type == "Zero")
                Cells1[j][k] = Cells1[j][k]+hit->GetEdep();
            else if (type == "Peak")
                Cells2[j][k] = Cells2[j][k]+hit->GetEdep();
        }
    }

    std::ofstream profileFile(type);

    G4double horizontal[101], vertical[101];
    for (G4int i = 0; i < 100; i++)
    {
        profileFile << "\n";
        for (G4int j = 0; j < 100; j++)
        {
            if (type == "Zero")
            {
                if ((i == 100)||(j == 100))
                {
                    profileFile << (G4double)(i*30.0)/100.0 << " " << (G4double)(j*30.0/100.0) << " 0 \n";
                }
                else
                    profileFile << i*30.0/100.0 << " " << j*30.0/100.0 << " " << Cells1[i][j] << "\n";

                if (i == 50)
                    horizontal[j] = Cells1[i][j];

                if (j == 50)
                    vertical[i] = Cells1[i][j];
            }

            else if (type == "Peak")
            {
                if ((i == 100)||(j == 100))
                {
                    profileFile << (G4double)(i*30.0)/100.0 << " " << (G4double)(j*30.0/100.0) << " 0 \n";
                }
                else
                    profileFile << i*30.0/100.0 << " " << j*30.0/100.0 << " " << Cells2[i][j] << "\n";

                if (i == 50)
                    horizontal[j] = Cells2[i][j];

                if (j == 50)
                    vertical[i] = Cells2[i][j];
            }
        }
    }

    horizontal[100] = 0;
    vertical[100] = 0;

    G4String name1 = "GEMProfile_H_"+type+".txt";
    G4String name2 = "GEMProfile_V_"+type+".txt";

    std::ofstream profileFileH(name1);
    std::ofstream profileFileV(name2);

    for (G4int box = 0; box <= 100; box++)
    {
        profileFileH << (G4double)(box*30.0/100.0) << " " << horizontal[box] << "\n";
        profileFileV << (G4double)(box*30.0/100.0) << " " << vertical[box] << "\n";
    }
}

