#include "GEMPhysicsList.hh"

// Physic lists (contained inside the Geant4 source code, in the 'physicslists folder')
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4ProcessManager.hh"
#include "G4IonFluctuations.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4EmProcessOptions.hh"
#include "G4ParallelWorldPhysics.hh"
#include "G4ParallelWorldScoringProcess.hh"
#include "PhysListEmStandardSingleSc.hh"

GEMPhysicsList::GEMPhysicsList() : G4VModularPhysicsList()
{
    defaultCutValue = 1.*mm;
    cutForGamma     = defaultCutValue;
    cutForElectron  = defaultCutValue;
    cutForPositron  = defaultCutValue;

    SetVerboseLevel(1);

    RegisterPhysics(new PhysListEmStandardSingleSc);
   // RegisterPhysics(new G4EmStandardPhysics_option4);
    RegisterPhysics(new G4HadronPhysicsQGSP_BIC);
    RegisterPhysics(new G4EmExtraPhysics);
    RegisterPhysics(new G4HadronElasticPhysics);
    RegisterPhysics(new G4StoppingPhysics);
    RegisterPhysics(new G4IonBinaryCascadePhysics);
    RegisterPhysics(new G4NeutronTrackingCut);
}

GEMPhysicsList::~GEMPhysicsList()
{}

void GEMPhysicsList::ConstructProcess()
{
    AddParallelScoring();
    G4VModularPhysicsList::ConstructProcess();
}

void GEMPhysicsList::AddParallelScoring()
{
    G4ParallelWorldScoringProcess* theParallelWorldScoringProcessDepth
            = new G4ParallelWorldScoringProcess("ParaWorldScoringProcDepth");
    theParallelWorldScoringProcessDepth->SetParallelWorld("GEMParallelWorld");

    G4ParallelWorldScoringProcess* theParallelWorldScoringProcessProfile
            = new G4ParallelWorldScoringProcess("ParaWorldScoringProcProfile");
    theParallelWorldScoringProcessProfile->SetParallelWorld("GEMVoxParallelWorld");

    theParticleIterator->reset();
    while((*theParticleIterator)())
    {
        G4ProcessManager* pmanager = theParticleIterator->value()->GetProcessManager();
        pmanager->AddProcess(theParallelWorldScoringProcessDepth);
        pmanager->AddProcess(theParallelWorldScoringProcessProfile);
        pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcessDepth, idxAtRest);
        pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcessProfile, idxAtRest);
        pmanager->SetProcessOrdering(theParallelWorldScoringProcessDepth, idxAlongStep, 1);
        pmanager->SetProcessOrdering(theParallelWorldScoringProcessProfile, idxAlongStep, 1);
        pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcessDepth, idxPostStep);
        pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcessProfile, idxPostStep);
    }
}

void GEMPhysicsList::SetCuts()
{
    // set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma
    SetCutValue(cutForGamma, "gamma");
    SetCutValue(cutForElectron, "e-");
    SetCutValue(cutForPositron, "e+");
}

