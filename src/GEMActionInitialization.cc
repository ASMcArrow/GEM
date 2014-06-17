#include "GEMActionInitialization.hh"
#include "GEMPrimaryGeneratorAction.hh"
#include "GEMRunAction.hh"
#include "GEMEventAction.hh"

GEMActionInitialization::GEMActionInitialization()
    : G4VUserActionInitialization()
{}

GEMActionInitialization::~GEMActionInitialization()
{}

void GEMActionInitialization::BuildForMaster() const
{
    SetUserAction(new GEMRunAction("FilmDetector"));
}

void GEMActionInitialization::Build() const
{
    SetUserAction(new GEMPrimaryGeneratorAction);
    SetUserAction(new GEMRunAction("FilmDetector"));
}
