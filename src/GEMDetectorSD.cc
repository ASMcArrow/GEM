#include "GEMDetectorSD.hh"
#include "GEMDetectorHit.hh"

#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4VProcess.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"

#include <cstdio>

GEMDetectorSD::GEMDetectorSD(G4String name):
    G4VSensitiveDetector(name)
{ 
    G4String HCname;
    collectionName.insert(HCname="GEMDetectorHitsCollection");
    HitsCollection = NULL;
    SensitiveDetectorName = name;
}

GEMDetectorSD::~GEMDetectorSD()
{}

void
GEMDetectorSD::Initialize(G4HCofThisEvent*)
{
    HitsCollection = new GEMDetectorHitsCollection(SensitiveDetectorName,
                                                           collectionName[0]);
}

G4bool
GEMDetectorSD::ProcessHits(G4Step* aStep, G4TouchableHistory* obsolete)
{
    G4StepPoint* preStep = aStep->GetPreStepPoint();
    G4TouchableHistory* touchable = (G4TouchableHistory*)(preStep->GetTouchable());

    G4int i = touchable->GetReplicaNumber(0);
    G4int j = touchable->GetReplicaNumber(1);

    G4double area = pow(3.0/100.0, 2);

 /* G4double i = aStep->GetPostStepPoint()->GetPosition().getX();
    G4double j = aStep->GetPostStepPoint()->GetPosition().getY();
    G4double k = aStep->GetPostStepPoint()->GetPosition().getZ(); */

    G4double energyDeposit = aStep->GetTotalEnergyDeposit();

    if(energyDeposit != 0)
	{
        GEMDetectorHit* detectorHit = new GEMDetectorHit();
        detectorHit->SetEdep(energyDeposit);
        detectorHit->SetPos(G4ThreeVector(i, j, 0));
        detectorHit->SetArea(area);
        HitsCollection->insert(detectorHit);

        return true;
    }

    return false;
}

void
GEMDetectorSD::EndOfEvent(G4HCofThisEvent* HCE)
{
    int HCID = GetCollectionID(0);
    HCE->AddHitsCollection(HCID, HitsCollection);
}

