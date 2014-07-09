#include "GEMFieldMessenger.hh"
#include "GEMMagneticField.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4SystemOfUnits.hh"

GEMFieldMessenger::GEMFieldMessenger(GEMMagneticField* field):MagneticField(field)
{
    // Set magnetic field
    GEMDir = new G4UIdirectory("/GEM/");
    GEMDir->SetGuidance("Directory for commands related to GEM setup");

    SetMagFieldCmd = new G4UIcmdWith3VectorAndUnit("/GEM/setMagField", this);
    SetMagFieldCmd->SetGuidance("Set the value of magnetic field inside the scanning magnet");
    SetMagFieldCmd->SetParameterName("X", "Y", "Z", false);
    SetMagFieldCmd->SetDefaultUnit("gauss");
    SetMagFieldCmd->AvailableForStates(G4State_Idle, G4State_PreInit);
}

GEMFieldMessenger::~GEMFieldMessenger()
{
    delete GEMDir;
    delete SetMagFieldCmd;
}

void GEMFieldMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    if( command == SetMagFieldCmd)
    {
        G4ThreeVector magfield = SetMagFieldCmd->GetNew3VectorValue(newValue);
        MagneticField->SetFieldValue(magfield);
    }
}
