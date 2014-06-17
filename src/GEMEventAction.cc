#include "GEMEventAction.hh"
#include "GEMDetectorHit.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Types.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Proton.hh"
#include "G4SDManager.hh"

#include <fstream>
#include <iostream>

GEMEventAction* GEMEventAction::m_Instance = 0;

GEMEventAction* GEMEventAction::Instance()
{
    return m_Instance;
}      

GEMEventAction::GEMEventAction():G4UserEventAction()
{

    m_Instance = this;
    hitsCollectionID = -1;

    for (int i = 0; i <= 600; i++)
    m_Bins[i] = 0;

    for (int i = 0; i <= 99; i++)
    m_Shells[i] = 0;
}

GEMEventAction::~GEMEventAction()
{
    m_Instance = 0;
}

void GEMEventAction::BeginOfEventAction(const G4Event* event)
{
    G4SDManager* pSDManager = G4SDManager::GetSDMpointer();
    if(hitsCollectionID == -1)
        hitsCollectionID = pSDManager->GetCollectionID("GEMDetectorHitsCollection");
}

void GEMEventAction::EndOfEventAction(const G4Event* event)
{
    if(hitsCollectionID < 0)
    return;

    G4HCofThisEvent* HCE = event->GetHCofThisEvent();

    if(HCE!=NULL)
    {
        GEMDetectorHitsCollection* CHC = (GEMDetectorHitsCollection*)(HCE -> GetHC(hitsCollectionID));
        if(CHC!=NULL)
        {
            G4int HitCount = CHC -> entries();

            for (G4int h=0; h<HitCount; h++)
            {
               /*G4double i = ((*CHC)[h])->GetZID();
               G4int roundi = floor(i + 0.5);
               m_Bins[roundi+300] = m_Bins[roundi+300]+((*CHC)[h])->GetEdep();*/

               G4int i = ((*CHC)[h])->GetPos()[0];
               m_Shells[i] = m_Shells[i]+((*CHC)[h])->GetEdep();
            }
        }
    }
}

