#ifndef GEMRUNACTION_H
#define GEMRUNACTION_H

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <vector>

class GEMRunAction : public G4UserRunAction
{
public:

    GEMRunAction(const G4String detectorName1, const G4String detectorName2);
    virtual ~GEMRunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

private:
    G4String DepthDetectorName, ProfileDetectorName;
    G4double Cells[100][100];
    G4int ScanVertical, ScanHorizontal;
};

#endif




