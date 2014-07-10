#ifndef GEMRUNACTION_H
#define GEMRUNACTION_H

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <vector>

class GEMRunAction : public G4UserRunAction
{
public:

    GEMRunAction(const G4String detectorName);
    virtual ~GEMRunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    G4double* GetCells()
    { return Cells; }

private:
    G4String DetectorName;
    G4double Cells[100];
};

#endif




