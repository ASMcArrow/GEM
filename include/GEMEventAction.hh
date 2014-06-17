#ifndef GEMEVENTACTION_H
#define GEMEVENTACTION_H

#include "G4UserEventAction.hh"
#include "G4LogicalVolume.hh"
#include "globals.hh"

class GEMEventAction : public G4UserEventAction
{

public:
    GEMEventAction();
    ~GEMEventAction();
    
    static GEMEventAction* Instance();

    void BeginOfEventAction(const G4Event* event);
    void EndOfEventAction(const G4Event* event);

    double* GetBins()
    { return m_Bins; }

    double* GetShells()
    { return m_Shells; }

private:
    static GEMEventAction* m_Instance;
    G4int hitsCollectionID;
    double m_Bins[601];
    double m_Shells[100];
};

#endif

    
