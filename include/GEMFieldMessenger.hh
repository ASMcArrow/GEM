#ifndef GEMFIELDMESSENGER_HH
#define GEMFIELDMESSENGER_HH

#include "GEMMagneticField.hh"

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;

class GEMFieldMessenger : public G4UImessenger
{

public:
    GEMFieldMessenger(GEMMagneticField* field);
    ~GEMFieldMessenger();

    void SetNewValue(G4UIcommand*, G4String);

private:
    GEMMagneticField* MagneticField;
    G4UIdirectory *GEMDir;

    G4UIcmdWith3VectorAndUnit *SetMagFieldCmd;
};


#endif // GEMFIELDMESSENGER_HH
