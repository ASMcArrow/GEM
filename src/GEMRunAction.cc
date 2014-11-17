#include "GEMRunAction.hh"
#include "GEMRun.hh"
#include "GEMDetectorHit.hh"
#include "GEMDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Run.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"
#include "G4Navigator.hh"
#include "G4UImanager.hh"
#include "DebugUISession.hh"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#define _USE_MATH_DEFINES

GEMRunAction::GEMRunAction(const G4String detectorName1, const G4String detectorName2, const G4String detectorName3) : G4UserRunAction()
{
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    DebugUISession * LoggedSession = new DebugUISession;
    UImanager->SetCoutDestination(LoggedSession);

    //DepthDetectorName = detectorName1;
    ProfileDetectorName1 = detectorName2;
    ProfileDetectorName2 = detectorName3;

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            Cells1[i][j] = 0;
            Cells2[i][j] = 0;
            Cells3[i][j] = 0;
        }

        Depth[i] = 0;
    }

    CurrentNHits = 0;
    PreviousNHits = 0;
}

GEMRunAction::~GEMRunAction()
{}

G4Run* GEMRunAction::GenerateRun()
{
    return new GEMRun(/*DepthDetectorName*/" ", ProfileDetectorName1, ProfileDetectorName2, 0);
}

void GEMRunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
    navigator->SetPushVerbosity(false);
    navigator->SetVerboseLevel(0);

    if (ScanVertical > 9)
    {
        ScanVertical = 0;
        ScanHorizontal++;
    }

    G4ThreeVector magField(0,0,0);
    magField.setX((G4double)(ScanHorizontal*((G4double)1500/9))-750);
    magField.setY((G4double)(ScanVertical*((G4double)1500/9))-750);
    magField.setZ(0);

    G4cout << "### Run " << aRun->GetRunID() << " ScanVertical = " << ScanVertical << " ScanHorizontal = " << ScanHorizontal << G4endl;
    G4cout << "Magnetic field " << magField.getX() << " " << magField.getY() << G4endl;
    GEMDetectorConstruction::MagneticField->SetFieldValue(magField);
}

void GEMRunAction::EndOfRunAction(const G4Run* aRun)
{
    ScanVertical++;

    GEMRun *gemRun = (GEMRun*)aRun;

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/tracking/verbose 0");
    UImanager->ApplyCommand("/event/verbose 0");

    PreviousNHits = CurrentNHits;
    CurrentNHits = gemRun->GetNumberOfHits("ProfileDetectorZero");

    if ((G4double)CurrentNHits < (G4double)(PreviousNHits/2))
    {
        UImanager->ApplyCommand("/tracking/verbose 0");
        UImanager->ApplyCommand("/event/verbose 1");
    }

    G4cout << "GEMRunAction: Number of events in this run " << gemRun->GetNumberOfEventToBeProcessed() << G4endl;
    G4cout << "GEMRunAction: Number of hits in this run in ZeroProfile detector " << gemRun->GetNumberOfHits("ProfileDetectorZero") << G4endl;
    if(!IsMaster()) return;


    //    G4int hitNum1 = gemRun->GetNumberOfHits("DepthDetector");
    //    G4cout << "Hits in DepthDetector = " << hitNum1 << G4endl;
    //    for (G4int i = 0; i < hitNum1; i++)
    //    {
    //        GEMDetectorHit* hit = (GEMDetectorHit*)(gemRun->GetHit("DepthDetector", i));
    //        if(hit != NULL)
    //        {
    //            G4int j = hit->GetPos()[0];
    //            Depth[j] = Depth[j]+hit->GetEdep();
    //        }
    //    }

    //    std::ofstream depthFile("GEMDepthMT.txt");
    //    for (G4int i = 0; i < 100; i++)
    //        depthFile << i*22.0/100.0 << " " << Depth[i]/Depth[0] << "\n";

    // Here is the temporary code for determining the distance between the points
    //    G4int hitNum = gemRun->GetNumberOfHits("ProfileDetectorIso");
    //    G4String detectorName = "ProfileDetectorIso";
    //    G4cout << "Hits in ProfileDetectorIso = " << hitNum << G4endl;

    //    for (G4int i = 0; i < hitNum; i++)
    //    {
    //        GEMDetectorHit* hit = (GEMDetectorHit*)(gemRun->GetHit(detectorName, i));

    //        if(hit != NULL)
    //        {
    //            G4int j = hit->GetPos()[0];
    //            G4int k = hit->GetPos()[1];
    //            Cells3[j][k] = Cells3[j][k]+hit->GetEdep();

    //            // G4cout << "i = " << j << " k = " << k << " edep = " << hit->GetEdep() << G4endl;
    //        }
    //    }

    //    G4double max = 0;
    //    G4int maxi = 0;
    //    G4int maxj = 0;

    //    for (G4int i = 0; i < 100; i++)
    //    {
    //        for (G4int j = 0; j < 100; j++)
    //        {
    //            if (max <= Cells3[i][j])
    //            {
    //                max = Cells3[i][j];
    //                maxi = i;
    //                maxj = j;
    //            }
    //        }
    //    }

    //    std::ofstream file;
    //    file.open("PointLocation.txt", std::ios_base::app | std::ios_base::out);
    //    file << maxi*30.0/100.0 << " " << maxj*30.0/100.0 << " " << max << "\n";;
    //    file.close();

    //    for (int i = 0; i < 100; i++)
    //    {
    //        for (int j = 0; j < 100; j++)
    //        {
    //            Cells3[i][j] = 0;
    //        }
    //    }

    this->DumpProfile("ZeroMT", gemRun);
    this->DumpProfile("IsoMT", gemRun);
}

void GEMRunAction::DumpProfile(G4String type, GEMRun *gemRun)
{
    G4int hitNum;
    G4String detectorName;
    G4String fileName;

    if (type == "ZeroMT")
    {
        hitNum = gemRun->GetNumberOfHits("ProfileDetectorZero");
        G4cout << "Hits in ProfileDetectorZero = " << hitNum << G4endl;
        detectorName = "ProfileDetectorZero";
        fileName = "GEMProfileZeroMT.txt";
    }
    else if (type == "IsoMT")
    {
        hitNum = gemRun->GetNumberOfHits("ProfileDetectorIso");
        G4cout << "Hits in ProfileDetectorIso = " << hitNum << G4endl;
        detectorName = "ProfileDetectorIso";
        fileName = "GEMProfileIsoMT.txt";
    }

    for (G4int i = 0; i < hitNum; i++)
    {
        GEMDetectorHit* hit = (GEMDetectorHit*)(gemRun->GetHit(detectorName, i));

        if(hit != NULL)
        {
            G4int j = hit->GetPos()[0];
            G4int k = hit->GetPos()[1];
            if (type == "ZeroMT")
                Cells1[j][k] = Cells1[j][k]+hit->GetEdep();
            else if (type == "IsoMT")
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
            if (type == "ZeroMT")
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

            else if (type == "IsoMT")
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

    G4String name1 = "MTGEMProfile_H_"+type+".txt";
    G4String name2 = "MTGEMProfile_V_"+type+".txt";

    std::ofstream profileFileH(name1);
    std::ofstream profileFileV(name2);

    for (G4int box = 0; box <= 100; box++)
    {
        profileFileH << (G4double)(box*30.0/100.0) << " " << horizontal[box] << "\n";
        profileFileV << (G4double)(box*30.0/100.0) << " " << vertical[box] << "\n";
    }
}

