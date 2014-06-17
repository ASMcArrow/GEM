#ifndef GEMMESSENGER_HH
#define GEMMESSENGER_HH

#include "G4UImessenger.hh"
#include "globals.hh"

class GEMDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

// Messenger class that implements commands:
// - /GEM/det/setSlabMaterialName name
// - /GEM/det/setSlabDimensions hx hy hz unit

class GEMDetectorMessenger: public G4UImessenger
{

public:
    GEMDetectorMessenger(GEMDetectorConstruction* );
    virtual ~GEMDetectorMessenger();

    virtual void SetNewValue(G4UIcommand* command, G4String newValue);

private:
    GEMDetectorConstruction* DetectorConstruction;
    G4UIdirectory* TopDirectory;
    G4UIdirectory* Directory;
    G4UIcmdWithAString* SetSlabMaterialCmd;
    G4UIcmdWithADoubleAndUnit* SetSlabThicknessCmd;
};

#endif // GEMMESSENGER_HH
