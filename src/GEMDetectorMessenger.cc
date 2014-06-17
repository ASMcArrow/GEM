#include "GEMDetectorMessenger.hh"
#include "GEMDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

GEMDetectorMessenger::GEMDetectorMessenger
(GEMDetectorConstruction* detectorConstruction)
    :G4UImessenger(), DetectorConstruction(detectorConstruction)
{
    TopDirectory = new G4UIdirectory("/GEM/");
    TopDirectory->SetGuidance("UI commands for GEM");

    Directory = new G4UIdirectory("/GEM/det/");
    Directory->SetGuidance("Control of material slab size and dimension");

    SetSlabMaterialCmd = new G4UIcmdWithAString("/GEM/det/setSlabMaterial", this);
    SetSlabMaterialCmd->SetGuidance("Select material of the slab");
    SetSlabMaterialCmd->SetParameterName("SlabMaterial", false);
    SetSlabMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    SetSlabThicknessCmd = new G4UIcmdWithADoubleAndUnit("/GEM/det/setSlabThickness",this);
    SetSlabThicknessCmd->SetGuidance("Set slab thickness (in half lentgh)");
    SetSlabThicknessCmd->SetParameterName("SlabThickness", false);
    SetSlabThicknessCmd->SetUnitCategory("Length");
    SetSlabThicknessCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

GEMDetectorMessenger::~GEMDetectorMessenger()
{
    delete TopDirectory;
    delete Directory;
    delete SetSlabMaterialCmd;
}

void GEMDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if(command == SetSlabMaterialCmd)
        DetectorConstruction->SetSlabMaterial(newValue);

    else if (command == SetSlabThicknessCmd)
    {
        G4double thickness = SetSlabThicknessCmd->ConvertToDimensionedDouble(newValue);
        DetectorConstruction->SetSlabThickness(thickness);
    }
}
