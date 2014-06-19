#include "GEMActionInitialization.hh"
#include "GEMPrimaryGeneratorAction.hh"

GEMActionInitialization::GEMActionInitialization()
    : G4VUserActionInitialization()
{}

GEMActionInitialization::~GEMActionInitialization()
{}

void GEMActionInitialization::BuildForMaster() const
{
    //SetUserAction(new GEMRunAction("FilmDetector"));
}

void GEMActionInitialization::Build() const
{
    SetUserAction(new GEMPrimaryGeneratorAction);
    //SetUserAction(new GEMRunAction("FilmDetector"));
}
