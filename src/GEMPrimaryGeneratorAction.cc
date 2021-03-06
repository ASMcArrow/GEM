#include "GEMPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4SingleParticleSource.hh"
#include "G4SPSAngDistribution.hh"
#include "G4Proton.hh"

using namespace CLHEP;

GEMPrimaryGeneratorAction::GEMPrimaryGeneratorAction()
{
    CircleSource = new G4SingleParticleSource();
    CircleSource->SetParticleDefinition(G4Proton::Definition());

    G4SPSEneDistribution* energy = CircleSource->GetEneDist();
    energy->SetEnergyDisType("Gauss");
    energy->SetBeamSigmaInE(1 *MeV);
    energy->SetMonoEnergy(152.3 *MeV);

    G4SPSPosDistribution* position = CircleSource->GetPosDist();
    position->SetPosDisType("Beam");
    //position->SetPosDisShape("Circle");
    //position->SetRadius(1.25*cm);
    position->SetCentreCoords(G4ThreeVector(0.0*m, 0.0*m, -280.0*cm - 1*cm));
    position->SetBeamSigmaInR(4.5*mm);

    G4SPSAngDistribution* angular = CircleSource->GetAngDist();
    angular->SetAngDistType("beam1d");
    angular->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));
    angular->DefineAngRefAxes("angref1", G4ThreeVector(-1.0,0.0,0.0));
    angular->SetBeamSigmaInAngR(0.003);

    CircleSource->SetNumberOfParticles(1);
}

GEMPrimaryGeneratorAction::~GEMPrimaryGeneratorAction()
{
    delete CircleSource;
}

void GEMPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    CircleSource->GeneratePrimaryVertex(event);
}
