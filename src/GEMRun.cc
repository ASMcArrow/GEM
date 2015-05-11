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
    }

    Verbose = verbose;

    Cells1 = new G4double*[100];
    for (int i = 0; i < 100; i++)
    {
        Cells1[i] = new G4double[100];
        for (int j = 0; j < 100; j++)
            Cells1[i][j] = 0;
    }

    Cells2 = new G4double*[100];
    for (int i = 0; i < 100; i++)
    {
        Cells2[i] = new G4double[100];
        for (int j = 0; j < 100; j++)
            Cells2[i][j] = 0;
    }

    for (int i = 0; i < 100; i++)
    {
        Depth[i] = 0;
    }
}

GEMRun::~GEMRun()
{
    for (int i = 0; i < 100; i++) {
        delete[] Cells1[i];
    }
    delete[] Cells1;

    for (int i = 0; i < 100; i++) {
        delete[] Cells2[i];
    }
    delete[] Cells2;
}

void GEMRun::RecordEvent(const G4Event* aEvent)
{
    numberOfEvent++;

    G4HCofThisEvent* HCE = aEvent->GetHCofThisEvent();
    if(HCE!=NULL)
    {
        GEMDetectorHitsCollection* HC1 = (GEMDetectorHitsCollection*)(HCE -> GetHC(IDVector[1]));
        if(HC1!=NULL)
        {
            for (G4int i = 0; i < HC1->entries(); i++)
            {
                GEMDetectorHit *hit = (GEMDetectorHit*)(HC1->GetHit(i));
                if (Verbose)
                {
                    G4cout << "HitsVector Initial: " << "i = "<< i << " Energy deposition is " << hit->GetEdep()
                           << " Position is" << hit->GetPos()[0] << G4endl;
                }
                G4int j = hit->GetPos()[0];
                G4int k = hit->GetPos()[1];

                Cells1[j][k] += hit->GetEdep();
            }
        }

        GEMDetectorHitsCollection* HC2 = (GEMDetectorHitsCollection*)(HCE -> GetHC(IDVector[2]));
        if(HC2!=NULL)
        {
            // G4cout << "Number of hits in this run in IsoProfile detector "
            //        << HC2->entries() << G4endl;
            for (G4int i = 0; i < HC2->entries(); i++)
            {
                GEMDetectorHit *hit = (GEMDetectorHit*)(HC2->GetHit(i));
                G4int j = hit->GetPos()[0];
                G4int k = hit->GetPos()[1];

                Cells2[j][k] += hit->GetEdep();
            }
        }

        GEMDetectorHitsCollection* HC = (GEMDetectorHitsCollection*)(HCE -> GetHC(IDVector[0]));
        if(HC!=NULL)
        {
            for (G4int i = 0; i < HC->entries(); i++)
            {
                GEMDetectorHit *hit = (GEMDetectorHit*)(HC->GetHit(i));
                G4int j = hit->GetPos()[0];
                Depth[j] += hit->GetEdep();
            }
        }
    }
}

void GEMRun::Merge(const G4Run * aRun)
{
    const GEMRun *localRun = static_cast<const GEMRun*>(aRun);

    G4cout << "Run " << localRun->GetRunID() << " is merged into run " << this->GetRunID() << G4endl;

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
            Cells1[i][j] += localRun->Cells1[i][j];
    }

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
            Cells2[i][j] += localRun->Cells2[i][j];
    }

    for (int i = 0; i < 100; i++)
    {
        Depth[i] += localRun->Depth[i];
    }

    G4Run::Merge(aRun);
}


