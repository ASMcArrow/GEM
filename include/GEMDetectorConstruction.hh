#ifndef GEMDETECTORCONSTRUCTION_H
#define GEMDETECTORCONSTRUCTION_H

#include "GEMMagneticField.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

using namespace CLHEP;

class GEMDetectorConstruction : public G4VUserDetectorConstruction
{

public:
    GEMDetectorConstruction() {}
    ~GEMDetectorConstruction() {}

    G4VPhysicalVolume* Construct();

    void ConstructSDandField();
    // This method is used in multi-threaded applications to build
    // per-worker non-shared objects: SensitiveDetectors and Field managers
    static G4ThreadLocal GEMMagneticField* MagneticField;

private:
    void InitializeMaterials();

    std::map <std::string, G4Material*> MaterialMap;
    G4LogicalVolume* MagnetLogic;
    G4LogicalVolume* PhantomLogic;
};

#endif

