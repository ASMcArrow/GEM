#include "GEMMagneticField.hh"
#include "GEMFieldMessenger.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4MTRunManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

GEMMagneticField::GEMMagneticField()
{
    Messenger = new GEMFieldMessenger(this);
    SetFieldValue(G4ThreeVector(0.,0.,0.));
}

GEMMagneticField::~GEMMagneticField()
{
    delete Messenger;
}

/*void GEMMagneticField::GetFieldValue(const G4double, G4double *Bfield) const
{
    Bfield[0] = fieldVal.x();
    Bfield[1] = fieldVal.y();
    Bfield[2] = fieldVal.z();
}*/

void GEMMagneticField::SetFieldValue(G4ThreeVector fVal)
{
    fieldVal = fVal;

/*#ifdef G4MULTITHREADED
    G4MTRunManager::GetMasterRunManager()->GeometryHasBeenModified(true);
#else
    G4RunManager::GetRunManager()->GeometryHasBeenModified(true);
#endif*/
    /* G4FieldManager* fMan = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    fMan->SetDetectorField(this);
    fMan->CreateChordFinder(this); */
}
