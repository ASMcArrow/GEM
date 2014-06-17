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

GEMParallelWorld::~GEMParallelWorld()
{}

void GEMParallelWorld::Construct()
{
    G4VPhysicalVolume *ghostWorld = GetWorld();
    G4LogicalVolume *worldLog = ghostWorld->GetLogicalVolume();

 // Place volumes in the parallel world here
    G4double detHalfDimension = 3.0*cm;
    G4double nDivisions = 100.0;
    G4double halfBoxWidth = (detHalfDimension/nDivisions);

 // The cylinder with concentric circles
    G4Box *ghostDetector = new G4Box("GhostDetectorBox", detHalfDimension, detHalfDimension, 0.25*cm);
    GhostDetectorLog = new G4LogicalVolume(ghostDetector, 0, "GhostDetectorLog");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0.25*cm), GhostDetectorLog, "GhostDetectorPhys", worldLog, 0, 0);

    G4VisAttributes* visAttributes = new G4VisAttributes;
    visAttributes->SetColor(0,0,1);
    GhostDetectorLog->SetVisAttributes(visAttributes);

 // Divide along X coordinate
    G4Box* ghostXBox = new G4Box("ghostXBox", halfBoxWidth, detHalfDimension, 0.25*cm);
    GhostXBoxLog = new G4LogicalVolume(ghostXBox, 0, "GhostXBoxLog");
    new G4PVReplica("GhostXBoxPhys", GhostXBoxLog, GhostDetectorLog, kXAxis, nDivisions, halfBoxWidth*2, 0);

 // Divide along Y coordinate
    G4Box* ghostYBox = new G4Box("ghostYBox", halfBoxWidth, halfBoxWidth, 0.25*cm);
    GhostYBoxLog = new G4LogicalVolume(ghostYBox, 0, "GhostYBoxLog");
    new G4PVReplica("GhostYBoxPhys", GhostYBoxLog, GhostXBoxLog, kYAxis, nDivisions, halfBoxWidth*2, 0);
}

void GEMParallelWorld::ConstructSD()
{
    G4SDManager* sDman = G4SDManager::GetSDMpointer();
    G4VSensitiveDetector* sDetector = new GEMDetectorSD("FilmDetector");
    sDman->AddNewDetector(sDetector);
    GhostYBoxLog->SetSensitiveDetector(sDetector);
}







// Division along X axis: the detector is divided in slices along the X axis

/*G4double halfXVoxelSizeX = halfDetectorSizeX/m_NumberOfVoxelsAlongX;
G4double halfXVoxelSizeY = halfDetectorSizeY;
G4double halfXVoxelSizeZ = halfDetectorSizeZ;
G4double voxelXThickness = 2*halfXVoxelSizeX;

G4Box *ghostXDivision = new G4Box("ghostXDivision",
                 halfXVoxelSizeX,
                 halfXVoxelSizeY,
                 halfXVoxelSizeZ);

G4LogicalVolume *ghostXDivisionLog = new G4LogicalVolume(ghostXDivision,
                               dummyMat,
                               "ghostXDivisionLog",
                               0,0,0);

G4VPhysicalVolume *ghostXDivisionPhys = new G4PVReplica("ghostXDivisionPhys",
                                                          ghostXDivisionLog,
                                                          ghostPhys,
                                                          kXAxis,
                                                          numberOfVoxelsAlongX,
                                                          voxelXThickness);*/

// Division along Y axis: the slices along the X axis are divided along the Y axis

/* G4double halfYVoxelSizeX = halfXVoxelSizeX;
G4double halfYVoxelSizeY = halfDetectorSizeY/numberOfVoxelsAlongY;
G4double halfYVoxelSizeZ = halfDetectorSizeZ;
G4double voxelYThickness = 2*halfYVoxelSizeY;

G4Box *ghostSector = new G4Box("ghostSector",
                halfYVoxelSizeX,
                halfYVoxelSizeY,
                halfYVoxelSizeZ);

G4LogicalVolume *ghostSectorLog = new G4LogicalVolume(ghostSector,
                               dummyMat,
                               "ghostSectorLog",
                               0,0,0);

G4VPhysicalVolume *ghostSectorPhys = new G4PVReplica("ghostSectorPhys",
                              ghostSectorLog,
                              ghostXDivisionPhys,
                              kYAxis,
                              numberOfVoxelsAlongY,
                              voxelYThickness);*/

// Division along Z axis: the slices along the Y axis are divided along the Z axis

/*  G4double halfZVoxelSizeX = m_DetectorSizeX;
G4double halfZVoxelSizeY = m_DetectorSizeY;
G4double halfZVoxelSizeZ = m_DetectorSizeZ/m_NumberOfVoxelsAlongZ;
G4double voxelZThickness = 2*halfZVoxelSizeZ;

G4Box *ghostZDivision = new G4Box("ghostZDivision",
                halfZVoxelSizeX,
                halfZVoxelSizeY,
                halfZVoxelSizeZ);

G4LogicalVolume *ghostZDivisionLog = new G4LogicalVolume(ghostZDivision,
                               dummyMat,
                               "ghostZDivisionLog",
                               0,0,0);
ghostZDivisionLog->SetSensitiveDetector(detector);

ghostZDivisionPhys = new G4PVReplica("ghostZDivisionPhys",
                                        ghostZDivisionLog,
                                        ghostPhys,
                                        kZAxis,
                                        m_NumberOfVoxelsAlongZ,
                                        voxelZThickness);*/
