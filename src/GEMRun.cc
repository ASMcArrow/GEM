#include "GEMRun.hh"
#include "G4SDManager.hh"

GEMRun::GEMRun(const std::vector<G4String> namevector, G4bool verbose) : G4Run()
{
    NameVector = namevector;
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    for (G4int i = 0; i < NameVector.size(); i++)
    {
        G4VSensitiveDetector* detector = SDman->FindSensitiveDetector(NameVector[i]);
        G4String collName = detector->GetCollectionName(0);
        G4int collectionID = SDman->GetCollectionID(collName);
        IDVector.push_back(collectionID);
        HitVectorVector.push_back(std::vector<GEMDetectorHit*> (0));
    }

    Verbose = verbose;
    Cells = new G4double*[200];
    for (int i = 0; i < 200; i++)
    {
        Cells[i] = new G4double[200];
        for (int j = 0; j < 200; j++)
            Cells[i][j] = 0;
    }
}

GEMRun::~GEMRun() {
    for (int i = 0; i < 200; i++) {
        delete[] Cells[i];
    }
    delete[] Cells;
}

void GEMRun::RecordEvent(const G4Event* aEvent)
{
    numberOfEvent++;

    G4HCofThisEvent* HCE = aEvent->GetHCofThisEvent();
    if(HCE!=NULL)
    {
        GEMDetectorHitsCollection* HC = (GEMDetectorHitsCollection*)(HCE -> GetHC(CollectionID));
        if(HC!=NULL)
        {
            //GEMDetectorHit *hit = 0;
            for (G4int i = 0; i < HC->entries(); i++)
            {
                GEMDetectorHit *hit = (GEMDetectorHit*)(HC->GetHit(i));
                if (Verbose)
                {
                    G4cout << "HitsVector Initial: " << "i = "<< i << " Energy deposition is " << hit->GetEdep()
                           << " Position is" << hit->GetPos()[0] << G4endl;
                }
                G4int j = hit->GetPos()[0];
                G4int k = hit->GetPos()[1];

                Cells[j][k] += hit->GetEdep()/hit->GetArea();
            }
        }
    }
}

void GEMRun::Merge(const G4Run * aRun)
{
    const GEMRun *localRun = static_cast<const GEMRun*>(aRun);
    for (int i = 0; i < 200; i++)
    {
        for (int j = 0; j < 200; j++)
            Cells[i][j] += localRun->Cells[i][j];
    }

    G4Run::Merge(aRun);
}


