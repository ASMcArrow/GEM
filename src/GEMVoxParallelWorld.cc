#include "GEMVoxParallelWorld.hh"
#include "GEMDetectorSD.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4PVReplica.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

GEMVoxParallelWorld::GEMVoxParallelWorld(G4String worldName)
    :G4VUserParallelWorld(worldName)
{
    // G4SDManager* sDman = G4SDManager::GetSDMpointer();
    // sDetector = new GEMDetectorSD("MarkusDetector");
    // sDman->AddNewDetector(sDetector);
}

void GEMVoxParallelWorld::Construct()
{
    G4VPhysicalVolume *ghostWorld = GetWorld();
    G4LogicalVolume *worldLog = ghostWorld->GetLogicalVolume();

    // Place volumes in the parallel world here
    G4double detHalfDimension = 15.0*cm;
    G4double nDivisions = 100.0;
    G4double halfBoxWidth = (detHalfDimension/nDivisions);

    // Profile at zero depth
    G4Box *ghostDetectorZero = new G4Box("GhostDetectorBoxZero", detHalfDimension, detHalfDimension, 0.25*cm);
    GhostDetectorZeroLog = new G4LogicalVolume(ghostDetectorZero, 0, "GhostDetectorZeroLog");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0.25*cm-11*cm), GhostDetectorZeroLog, "GhostDetectorZeroPhys", worldLog, 0, 0);

    G4VisAttributes* visAttributesZero = new G4VisAttributes;
    visAttributesZero->SetColor(0,0,1);
    GhostDetectorZeroLog->SetVisAttributes(visAttributesZero);

  /*  // Divide along X coordinate at zero depth
    G4Box* ghostXBoxZero = new G4Box("ghostXBoxZero", halfBoxWidth, detHalfDimension, 0.25*cm);
    G4LogicalVolume *GhostXBoxZeroLog = new G4LogicalVolume(ghostXBoxZero, 0, "GhostXBoxZeroLog");
    new G4PVReplica("GhostXBoxZeroPhys", GhostXBoxZeroLog, GhostDetectorZeroLog, kXAxis, nDivisions, halfBoxWidth*2, 0);

    // Divide along Y coordinate at zero depth
    G4Box* ghostYBoxZero = new G4Box("ghostYBoxZero", halfBoxWidth, halfBoxWidth, 0.25*cm);
    GhostYBoxZeroLog = new G4LogicalVolume(ghostYBoxZero, 0, "GhostYBoxZeroLog");
    new G4PVReplica("GhostYBoxZeroPhys", GhostYBoxZeroLog, GhostXBoxZeroLog, kYAxis, nDivisions, halfBoxWidth*2, 0);*/

    // Profile at peak depth
    G4Box *ghostDetectorPeak = new G4Box("GhostDetectorBoxPeak", detHalfDimension, detHalfDimension, 0.25*cm);
    GhostDetectorPeakLog = new G4LogicalVolume(ghostDetectorPeak, 0, "GhostDetectorPeakLog");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0*cm), GhostDetectorPeakLog, "GhostDetectorPeakPhys", worldLog, 0, 0);
    //new G4PVPlacement(0, G4ThreeVector(0, 0, 5.0*cm), GhostDetectorPeakLog, "GhostDetectorPeakPhys", worldLog, 0, 0);

    G4VisAttributes* visAttributesPeak = new G4VisAttributes;
    visAttributesPeak->SetColor(0,1,0);
    GhostDetectorZeroLog->SetVisAttributes(visAttributesPeak);

   /* // Divide along X coordinate at peak depth
    G4Box* ghostXBoxPeak = new G4Box("ghostXBoxPeak", halfBoxWidth, detHalfDimension, 1*cm);
    G4LogicalVolume *GhostXBoxPeakLog = new G4LogicalVolume(ghostXBoxPeak, 0, "GhostXBoxPeakLog");
    new G4PVReplica("GhostXBoxPeakPhys", GhostXBoxPeakLog, GhostDetectorPeakLog, kXAxis, nDivisions, halfBoxWidth*2, 0);

    // Divide along Y coordinate at peak depth
    G4Box* ghostYBoxPeak = new G4Box("ghostYBoxPeak", halfBoxWidth, halfBoxWidth, 1*cm);
    GhostYBoxPeakLog = new G4LogicalVolume(ghostYBoxPeak, 0, "GhostYBoxPeakLog");
    new G4PVReplica("GhostYBoxPeakPhys", GhostYBoxPeakLog, GhostXBoxPeakLog, kYAxis, nDivisions, halfBoxWidth*2, 0);*/
}

void GEMVoxParallelWorld::ConstructSD()
{
    G4SDManager* sDman = G4SDManager::GetSDMpointer();
    ZeroDetector = new GEMDetectorSD("ProfileDetectorZero", "ProfileHitsCollectionZero");
    sDman->AddNewDetector(ZeroDetector);
    GhostDetectorZeroLog->SetSensitiveDetector(ZeroDetector);
   // GhostYBoxZeroLog->SetSensitiveDetector(ZeroDetector);

    PeakDetector = new GEMDetectorSD("ProfileDetectorPeak", "ProfileHitsCollectionPeak");
    sDman->AddNewDetector(PeakDetector);
    GhostDetectorPeakLog->SetSensitiveDetector(PeakDetector);
   // GhostYBoxPeakLog->SetSensitiveDetector(PeakDetector);
}

