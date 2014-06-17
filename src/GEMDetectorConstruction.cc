#include "GEMDetectorConstruction.hh"
#include "GEMDetectorMessenger.hh"
#include "GEMDetectorSD.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4NistManager.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "G4VSolid.hh"

GEMDetectorConstruction* GEMDetectorConstruction::Instance = 0;

GEMDetectorConstruction::GEMDetectorConstruction()
{
    Instance = this;
    SlabThickness = 4.375*cm;
    DetectorMessenger = new GEMDetectorMessenger(this);
}

GEMDetectorConstruction* GEMDetectorConstruction::GetInstance()
{
    return Instance;
}

G4VPhysicalVolume* GEMDetectorConstruction::Construct()
{
 // Cleanup old geometry
    /*G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();*/

    this->InitializeMaterials();

    G4VisAttributes* visAttributes = new G4VisAttributes;
    visAttributes->SetForceWireframe(true);

 // World
    G4Box* world = new G4Box("World", 3*m, 3*m, 3*m);
    G4LogicalVolume *worldLogic = new G4LogicalVolume(world, MaterialMap["Air"], "WorldLogic");
    G4VPhysicalVolume *worldPhys = new G4PVPlacement(0, G4ThreeVector(), worldLogic, "WorldPhys", 0, false, 0);
    worldLogic->SetVisAttributes(visAttributes);

 // Material Slab
    /*G4Box *materialSlab = new G4Box("MaterialSlab", 5*cm, 5*cm, SlabThickness);
    MaterialLogic = new G4LogicalVolume(materialSlab, MaterialMap["Water"], "MaterialLogic");
    new G4PVPlacement(0, G4ThreeVector(0, 0, -80.2*cm + SlabThickness), MaterialLogic, "MaterialSlab", worldLogic, 0, 0);*/

 // Collimator
    G4Tubs *collimator = new G4Tubs("Collimator", 0.15*cm, 15*cm, 5*cm, 0, 2*CLHEP::pi);
    G4LogicalVolume *steelLogic = new G4LogicalVolume(collimator, MaterialMap["StainlessSteel"], "SteelLogic");
    new G4PVPlacement(0, G4ThreeVector(0, 0, -80.2*cm - 5*cm), steelLogic, "Collimator", worldLogic, 0, 0);

 // Detector
    G4Box *detector = new G4Box("DetectorBox", 3*cm, 3*cm, 0.25*cm);
    G4LogicalVolume *detectorLogic = new G4LogicalVolume(detector, MaterialMap["Water"], "DetectorLogic");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0.25*cm), detectorLogic, "DetectorPhys", worldLogic, 0, 0);

    return worldPhys;
}

void GEMDetectorConstruction::SetSlabMaterial(G4String materialName)
{
    MaterialLogic->SetMaterial(MaterialMap[materialName]);
}

void GEMDetectorConstruction::SetSlabThickness(G4double thickness)
{
    SlabThickness = thickness;
}

void GEMDetectorConstruction::UpdateGeometry()
{
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

void GEMDetectorConstruction::InitializeMaterials()
{
    G4NistManager* nistManager = G4NistManager::Instance();

    G4Element* H = nistManager->FindOrBuildElement(1);
    G4Element* C = nistManager->FindOrBuildElement(6);
    G4Element* O = nistManager->FindOrBuildElement(8);
    G4Element* N = nistManager->FindOrBuildElement(7);
    G4Element* Mg = nistManager->FindOrBuildElement(12);
    G4Element* P = nistManager->FindOrBuildElement(15);
    G4Element* S = nistManager->FindOrBuildElement(16);
    G4Element* Ca = nistManager->FindOrBuildElement(20);
    G4Element* Zn = nistManager->FindOrBuildElement(30);
    G4Element* Na = nistManager->FindOrBuildElement(11);
    G4Element* K = nistManager->FindOrBuildElement(19);
    G4Element* Cl = nistManager->FindOrBuildElement(17);
    G4Element* Fe = nistManager->FindOrBuildElement(26);
    G4Element* Si = nistManager->FindOrBuildElement(14);
    G4Element* Cr = nistManager->FindOrBuildElement(24);
    G4Element* Mn = nistManager->FindOrBuildElement(12);
    G4Element* Ni = nistManager->FindOrBuildElement(28);

    G4Material* Air = nistManager->FindOrBuildMaterial("G4_AIR");
    MaterialMap["Air"] = Air;

    G4Material* Water = nistManager->FindOrBuildMaterial("G4_WATER");
    MaterialMap["Water"] = Water;

    G4Material* PMMA = new G4Material("PMMA", 1.17*g/cm3, 3);
    PMMA -> AddElement(H, 8);
    PMMA -> AddElement(C, 5);
    PMMA -> AddElement(O, 2);
    MaterialMap["PMMA"] = PMMA;

    G4Material* ICRUCompactBone = new G4Material("ICRUCompactBone", 1.85*g/cm3, 8);
    ICRUCompactBone->AddElement(H, 6.3984*perCent);
    ICRUCompactBone->AddElement(C, 27.8000*perCent);
    ICRUCompactBone->AddElement(N, 2.7000*perCent);
    ICRUCompactBone->AddElement(O, 41.0016*perCent);
    ICRUCompactBone->AddElement(Mg, 0.2000*perCent);
    ICRUCompactBone->AddElement(P, 7.0000*perCent);
    ICRUCompactBone->AddElement(S, 0.2000*perCent);
    ICRUCompactBone->AddElement(Ca, 14.7000*perCent);
    MaterialMap["ICRUCompactBone"] = ICRUCompactBone;

    G4Material* ICRPCompactBone = new G4Material("ICRPCompactBone", 1.85*g/cm3, 9);
    ICRPCompactBone->AddElement(H, 4.7234*perCent);
    ICRPCompactBone->AddElement(C, 14.4330*perCent);
    ICRPCompactBone->AddElement(N, 4.1990*perCent);
    ICRPCompactBone->AddElement(O, 44.6096*perCent);
    ICRPCompactBone->AddElement(Mg, 0.2200*perCent);
    ICRPCompactBone->AddElement(P, 10.4970*perCent);
    ICRPCompactBone->AddElement(S, 0.3150*perCent);
    ICRPCompactBone->AddElement(Ca, 20.9930*perCent);
    ICRPCompactBone->AddElement(Zn, 0.01*perCent);
    MaterialMap["ICRPCompactBone"] = ICRPCompactBone;

    G4Material* LungWW = new G4Material("LungWW", 0.20*g/cm3, 9);
    LungWW->AddElement(C, 10.5*perCent);
    LungWW->AddElement(O, 74.9*perCent);
    LungWW->AddElement(H, 10.3*perCent);
    LungWW->AddElement(N, 3.1*perCent);
    LungWW->AddElement(Na, 0.1*perCent);
    LungWW->AddElement(P, 0.2*perCent);
    LungWW->AddElement(K, 0.2*perCent);
    LungWW->AddElement(Cl, 0.3*perCent);
    LungWW->AddElement(S, 0.3*perCent);
    MaterialMap["LungWW"] = LungWW;

    G4Material* ICRPAdipose = new G4Material("ICRPAdipose", 0.92*g/cm3, 13);
    ICRPAdipose->AddElement(H, 11.9477*perCent);
    ICRPAdipose->AddElement(C, 63.7240*perCent);
    ICRPAdipose->AddElement(N, 0.7970*perCent);
    ICRPAdipose->AddElement(O, 23.2333*perCent);
    ICRPAdipose->AddElement(Na, 0.0500*perCent);
    ICRPAdipose->AddElement(Mg, 0.0020*perCent);
    ICRPAdipose->AddElement(P, 0.0160*perCent);
    ICRPAdipose->AddElement(S, 0.0730*perCent);
    ICRPAdipose->AddElement(Cl, 0.1190*perCent);
    ICRPAdipose->AddElement(K, 0.0320*perCent);
    ICRPAdipose->AddElement(Ca, 0.0020*perCent);
    ICRPAdipose->AddElement(Fe, 0.0020*perCent);
    ICRPAdipose->AddElement(Zn, 0.0020*perCent);
    MaterialMap["ICRPAdipose"] = ICRPAdipose;

    G4Material* ICRPMuscle = new G4Material("ICRPMuscle", 1.04*g/cm3, 13);
    ICRPMuscle->AddElement(H, 10.0637*perCent);
    ICRPMuscle->AddElement(C, 10.7830*perCent);
    ICRPMuscle->AddElement(N, 2.7680*perCent);
    ICRPMuscle->AddElement(O, 75.4773*perCent);
    ICRPMuscle->AddElement(Na, 0.0750*perCent);
    ICRPMuscle->AddElement(Mg, 0.0190*perCent);
    ICRPMuscle->AddElement(P, 0.1800*perCent);
    ICRPMuscle->AddElement(S, 0.2410*perCent);
    ICRPMuscle->AddElement(Cl, 0.0790*perCent);
    ICRPMuscle->AddElement(K, 0.3020*perCent);
    ICRPMuscle->AddElement(Ca, 0.0030*perCent);
    ICRPMuscle->AddElement(Fe, 0.0040*perCent);
    ICRPMuscle->AddElement(Zn, 0.0050*perCent);
    MaterialMap["ICRPMuscle"] = ICRPMuscle;

    G4Material* ICRUMuscle = new G4Material("ICRUMuscle", 1.04*g/cm3, 9);
    ICRUMuscle->AddElement(H, 10.1997*perCent);
    ICRUMuscle->AddElement(C, 12.3000*perCent);
    ICRUMuscle->AddElement(N, 3.5000*perCent);
    ICRUMuscle->AddElement(O, 72.9003*perCent);
    ICRUMuscle->AddElement(Na, 0.0800*perCent);
    ICRUMuscle->AddElement(Mg, 0.0200*perCent);
    ICRUMuscle->AddElement(P, 0.2000*perCent);
    ICRUMuscle->AddElement(S, 0.5000*perCent);
    ICRUMuscle->AddElement(K, 0.3000*perCent);
    MaterialMap["ICRUMuscle"] = ICRUMuscle;

    G4Material* StainlessSteel = new G4Material("StainlessSteel", 8.06*g/cm3, 6);
    StainlessSteel->AddElement(C, 0.001);
    StainlessSteel->AddElement(Si, 0.007);
    StainlessSteel->AddElement(Cr, 0.18);
    StainlessSteel->AddElement(Mn, 0.01);
    StainlessSteel->AddElement(Fe, 0.712);
    StainlessSteel->AddElement(Ni, 0.09);
    MaterialMap["StainlessSteel"] = StainlessSteel;
}


