#include "GEMMagneticField.hh"
#include "GEMFieldMessenger.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4MTRunManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

GEMMagneticField::GEMMagneticField():G4UniformMagField(G4ThreeVector(0., 0., 0.))
{
    Messenger = new GEMFieldMessenger(this);
    fieldVal = G4ThreeVector(0., 0., 0.);
}

GEMMagneticField::~GEMMagneticField()
{
    delete Messenger;
}

void GEMMagneticField::GetFieldValue(const G4double unused[4], G4double *Bfield) const
{
    Bfield[0] = fieldVal.x();
    Bfield[1] = fieldVal.y();
    Bfield[2] = fieldVal.z();
}

void GEMMagneticField::SetFieldValue(const G4ThreeVector &newFieldValue)
{
    fieldVal = newFieldValue;
    G4FieldManager* fMan = new G4FieldManager;
    fMan->SetDetectorField(this);
    fMan->CreateChordFinder(this);

    G4TransportationManager* tMan = G4TransportationManager::GetTransportationManager();
    std::vector<G4VPhysicalVolume*>::iterator it = tMan->GetWorldsIterator();
    for(int i = 0; i < tMan->GetNoWorlds(); i++)
    {
        if((*it)->GetName() == "WorldPhys")
        {
            (*it)->GetLogicalVolume()->GetDaughter(9)->GetLogicalVolume()->SetFieldManager(fMan, true);
        }
    }

     /*#ifndef G4MULTITHREADED
            G4MTRunManager::GetMasterRunManager()->GeometryHasBeenModified();
        #else
            G4RunManager::GetRunManager()->GeometryHasBeenModified();
        #endif*/
}
