#include "DebugTrajectory.hh"
#include "DebugTrajectoryPoint.hh"

G4ThreadLocal G4Allocator<DebugTrajectory> *DebugTrajectoryAllocator = 0;

DebugTrajectory::DebugTrajectory() :
    PositionRecord(0),
    ParentID(0),
    PDGCharge(0.0),
    ParticleName(""),
    InitialKineticEnergy( 0. ),
    InitialMomentum( G4ThreeVector())
{}

DebugTrajectory::DebugTrajectory(const G4Track* aTrack)
{
    G4ParticleDefinition *particleDefinition = aTrack->GetDefinition();
    ParticleName = particleDefinition->GetParticleName();
    PDGCharge = particleDefinition->GetPDGCharge();
    ParentID = aTrack->GetParentID();
    InitialKineticEnergy = aTrack->GetKineticEnergy();
    InitialMomentum = aTrack->GetMomentum();
    PositionRecord = new TrajectoryPointContainer();

    // Following is for the first trajectory point
    PositionRecord->push_back(new DebugTrajectoryPoint(aTrack->GetPosition(),
                                                       aTrack->GetKineticEnergy(), 0));
}

DebugTrajectory::DebugTrajectory(DebugTrajectory &right)
{
    ParticleName = right.ParticleName;
    PDGCharge = right.PDGCharge;
    ParentID = right.ParentID;
    InitialKineticEnergy = right.InitialKineticEnergy;
    InitialMomentum = right.InitialMomentum;
    PositionRecord = new TrajectoryPointContainer();

    for(size_t i = 0; i < right.PositionRecord->size(); i++)
    {
        DebugTrajectoryPoint* rightPoint = (DebugTrajectoryPoint*)((*(right.PositionRecord))[i]);
        PositionRecord->push_back(new DebugTrajectoryPoint(*rightPoint));
    }
}

DebugTrajectory::~DebugTrajectory()
{
    if (PositionRecord)
    {
        size_t i;
        for(i = 0; i < PositionRecord->size(); i++)
        {
            delete  (*PositionRecord)[i];
        }
        PositionRecord->clear();
        delete PositionRecord;
    }
}


void DebugTrajectory::AppendStep(const G4Step* aStep)
{
    PositionRecord->push_back(new DebugTrajectoryPoint(aStep->GetPostStepPoint()->GetPosition(),
                                                       aStep->GetTrack()->GetKineticEnergy(),
                                                       aStep->GetTotalEnergyDeposit()));
}

void DebugTrajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
    if(!secondTrajectory) return;

    DebugTrajectory* seco = (DebugTrajectory*)secondTrajectory;
    G4int ent = seco->GetPointEntries();
    for(G4int i = 1; i < ent; i++) // initial point of the second trajectory should not be merged
    {
        PositionRecord->push_back((*(seco->PositionRecord))[i]);
    }
    delete (*seco->PositionRecord)[0];
    seco->PositionRecord->clear();
}

void DebugTrajectory::ShowTrajectory(std::ostream& os) const
{
    std::ofstream file;
    file.open("Trajectories.txt", std::ios_base::app | std::ios_base::out);
    file << "\n";
    file << "Particle name is " << this->ParticleName << "\n";
    file << "Parent ID is " << this->ParentID << "\n";
    file << "Charge is " << this->PDGCharge << "\n";
    file << "Initial kinetic energy is " << this->InitialKineticEnergy << "\n";
    file << "Initial momentum is " << this->InitialMomentum[0] << " "
         << this->InitialMomentum[1] << " "
         << this->InitialMomentum[2] << "\n";

    G4int ent = this->GetPointEntries();
    for(G4int i = 0; i < ent; i++)
    {
        file << "Point 1: \n";
        file << "Position is " << ((DebugTrajectoryPoint*)(*PositionRecord)[i])->GetPosition()[0] << " "
             << ((DebugTrajectoryPoint*)(*PositionRecord)[i])->GetPosition()[1] << " "
             << ((DebugTrajectoryPoint*)(*PositionRecord)[i])->GetPosition()[2] << "\n";
        file << "Energy deposition is " << ((DebugTrajectoryPoint*)(*PositionRecord)[i])->GetEdep() << "\n";
        file << "Current kinetic energy is " << ((DebugTrajectoryPoint*)(*PositionRecord)[i])->GetEnergy() << "\n";
    }
    file.close();
}

void DebugTrajectory::DrawTrajectory() const
{
    // Invoke the default implementation in G4VTrajectory...
    G4VTrajectory::DrawTrajectory();
    // ... or override with your own code here.
}
