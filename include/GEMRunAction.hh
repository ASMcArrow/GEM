#ifndef GEMRUNACTION_H
#define GEMRUNACTION_H

#include "GEMRun.hh"

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <vector>

class GEMRunAction : public G4UserRunAction
{
public:

    GEMRunAction(const std::vector<G4String> nameVector);
    virtual ~GEMRunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

private:
    void DumpProfile(G4String type, GEMRun *gemRun);

    std::vector<G4String> NameVector;

    G4int ScanVertical, ScanHorizontal;
    G4ThreeVector MagField;
    G4double Cells1[100][100], Cells2[100][100], Depth[100];
};

#endif




