#ifndef GEMMAGNETICFIELD_HH
#define GEMMAGNETICFIELD_HH

#include "GEMFieldMessenger.hh"

#include "G4UniformMagField.hh"
#include "G4ThreeVector.hh"

class GEMMagneticField : public G4UniformMagField
{
public:

    GEMMagneticField();
    virtual ~GEMMagneticField();

    virtual void GetFieldValue(G4double *Bfield) const;
    void SetFieldValue(const G4ThreeVector& newFieldValue);

private:

    GEMFieldMessenger* Messenger;
    G4ThreeVector fieldVal;
};

#endif // GEMMAGNETICFIELD_HH
