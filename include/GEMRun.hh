#ifndef GEMRUN_H
#define GEMRUN_H

#include "G4Run.hh"
#include "G4Event.hh"

#include "GEMDetectorHit.hh"

#include <vector>

class GEMRun : public G4Run
{
public:

  GEMRun(const G4String detectorName, G4bool verbose);
  virtual ~GEMRun();

  // virtual method from G4Run.
  // The method is overriden in this class for scoring.
  virtual void RecordEvent(const G4Event*);
  virtual void Merge(const G4Run*);

  G4int GetNumberOfHits() const { return HitVector.size();}
  GEMDetectorHit* GetHit(G4int i) {return HitVector[i];}

private:
  G4String CollName;
  G4int CollectionID;
  std::vector<GEMDetectorHit*> HitVector;
  G4bool Verbose;
};

#endif // GEMRUN_H


