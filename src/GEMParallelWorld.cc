#include "GEMParallelWorld.hh"
#include "GEMDetectorSD.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4PVReplica.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

GEMParallelWorld::GEMParallelWorld(G4String worldName)
    :G4VUserParallelWorld(worldName)
{}

void GEMParallelWorld::Construct()
{
    G4VPhysicalVolume *ghostWorld = GetWorld();
    G4LogicalVolume *worldLog = ghostWorld->GetLogicalVolume();

 // Place volumes in the parallel world here
    G4double detectorR = 2.5*mm;
    G4int nDivisions = 100;

 // Tube in the centre of the phantom
    G4Tubs *ghostTube = new G4Tubs("ghostTube", 0, detectorR, 11*cm, 0, CLHEP::twopi);
    GhostTubeLog = new G4LogicalVolume(ghostTube, 0, "GhostTubeLog");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), GhostTubeLog, "GhostTubePhys", worldLog, 0, 0);

    G4VisAttributes* visAttributes = new G4VisAttributes;
    visAttributes->SetColor(0,0,1);
    GhostTubeLog->SetVisAttributes(visAttributes);

 // Division along Z axis
    G4Tubs* ghostZTube = new G4Tubs("ghostTube", 0, detectorR, 11*cm/nDivisions, 0, CLHEP::twopi);
    GhostZTubeLog = new G4LogicalVolume(ghostZTube, 0, "GhostZBoxLog");
    new G4PVReplica("GhostZBoxPhys", GhostZTubeLog, GhostTubeLog, kZAxis, nDivisions, (11*cm/nDivisions)*2, 0);
}

void GEMParallelWorld::ConstructSD()
{
    G4SDManager* sDman = G4SDManager::GetSDMpointer();
    sDetector = new GEMDetectorSD("DepthDetector", "DepthHitsCollection");
    sDman->AddNewDetector(sDetector);
    GhostZTubeLog->SetSensitiveDetector(sDetector);
}
