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

GEMRunAction::GEMRunAction(const std::vector<G4String> nameVector) : G4UserRunAction()
{
    //    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    //    DebugUISession * LoggedSession = new DebugUISession;
    //    UImanager->SetCoutDestination(LoggedSession);

    NameVector = nameVector;

    ScanVertical = 0;
    ScanHorizontal = 0;

    MagField.set(0,0,0);

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
            Cells1[i][j] = 0;
    }
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
            Cells2[i][j] = 0;
    }
    for (int i = 0; i < 100; i++)
    {
        Depth[i] = 0;
    }
}

GEMRunAction::~GEMRunAction()
{}

G4Run* GEMRunAction::GenerateRun()
{
    return new GEMRun(NameVector, 0);
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
        if (ScanHorizontal > 9)
            ScanHorizontal = 0;
    }

    MagField.setX((G4double)((ScanHorizontal*((G4double)1900/9))-950)*gauss);
    MagField.setY((G4double)((ScanVertical*((G4double)1900/9))-950)*gauss);
    MagField.setZ(0);

    GEMDetectorConstruction::MagneticField->SetFieldValue(MagField);

    G4double foo[4];
    G4double field[3];
    GEMDetectorConstruction::MagneticField->GetFieldValue(foo, field);
    G4cout << "### Run " << aRun->GetRunID() << " ScanVertical = " << ScanVertical << " ScanHorizontal = " << ScanHorizontal << G4endl;
    G4cout << "Magnetic field " << field[0] << " " << field[1] << G4endl;
}

void GEMRunAction::EndOfRunAction(const G4Run* aRun)
{
    ScanVertical++;

    GEMRun *gemRun = (GEMRun*)aRun;

    if(!IsMaster()) return;

    if (ScanHorizontal >= 7)
    {
        for (G4int i = 0; i < 100; i++)
            Depth[i] += gemRun->GetDepth()[i];

        std::ofstream depthFile("GEMDepthMT.txt");
        for (G4int i = 0; i < 100; i++)
            depthFile << i*22.0/100.0 << " " << Depth[i]/Depth[0] << "\n";
    }

    // Here is the temporary code for determining the distance between the points
    //    G4double max = 0;
    //    G4int maxi = 0;
    //    G4int maxj = 0;
    //    G4double** point = gemRun->GetCells2();

    //    for (G4int i = 0; i < 100; i++)
    //    {
    //        for (G4int j = 0; j < 100; j++)
    //        {
    //            if (max <= point[i][j])
    //            {
    //                max = point[i][j];
    //                maxi = i;
    //                maxj = j;
    //            }
    //        }
    //    }

    //    std::ofstream file;
    //    file.open("PointCalibration.txt", std::ios_base::app | std::ios_base::out);
    //    file << MagField[1] << " " << maxi*30.0/100.0 << " " << maxj*30.0/100.0 << " " << max << "\n";
    //    if (ScanVertical == 10)
    //        file << "\n";
    //    file.close();

    //    for (int i = 0; i < 100; i++)
    //    {
    //        for (int j = 0; j < 100; j++)
    //        {
    //            point[i][j] = 0;
    //        }
    //    }

    this->DumpProfile("ZeroMT", gemRun);
    this->DumpProfile("IsoMT", gemRun);
}

void GEMRunAction::DumpProfile(G4String type, GEMRun *gemRun)
{
    G4String detectorName;
    G4String fileName;

    if (type == "ZeroMT")
    {
        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 100; j++)
                Cells1[i][j] += gemRun->GetCells1()[i][j];
        }
        detectorName = "ProfileDetectorZero";
        fileName = "GEMProfileZeroMT.txt";
    }
    else if (type == "IsoMT")
    {
        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 100; j++)
            {
                Cells2[i][j] += gemRun->GetCells2()[i][j];
            }
        }
        detectorName = "ProfileDetectorIso";
        fileName = "GEMProfileIsoMT.txt";
    }

    std::ofstream profileFile(fileName);
    G4double horizontal[101], vertical[101];

    if (type == "ZeroMT")
    {
        for (G4int i = 0; i <= 100; i++)
        {
            profileFile << "\n";
            for (G4int j = 0; j <= 100; j++)
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
        }
    }
    else if (type == "IsoMT")
    {
        G4double horizontal[101], vertical[101];
        for (G4int i = 0; i <= 100; i++)
        {
            profileFile << "\n";
            for (G4int j = 0; j <= 100; j++)
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

