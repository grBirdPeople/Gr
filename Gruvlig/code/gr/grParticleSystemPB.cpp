#include	"grParticleSystemPB.h"

#include	"grParticleManagerPB.h"
#include	"grV2.h"
#include	"grBBox.h"
#include	"grDebugManager.h"


// Update
//////////////////////////////////////////////////
void
grParticleSystemPB::Update( grParticleSetupPB& rParticleSetup, const float fixedT )
{
    vecParticle& rVec = rParticleSetup.VecParticle;
    grParticleAttributePB& rAttribute = *rParticleSetup.pParticleAttribute;
    for ( uInt idx = 0; idx < rParticleSetup.ParticlesActive; ++idx )
    {
        grParticlePB& rPart = *rParticleSetup.VecParticle[ idx ].get();

        rPart.Acceleration += rAttribute.Gravity / rVec[ idx ]->Mass;

        rPart.Velocity += rPart.Acceleration * fixedT;
        rPart.Position += rPart.Velocity * fixedT;

        rPart.LifeTime -= fixedT;
        rPart.Acceleration = grV2f();

        // TEST
        std::cout << " Particles active: " << rParticleSetup.ParticlesActive << '\n';
        grBBox box( grV2f( 20.0f, 20.0f ), rPart.Position );
        grDebugManager::Instance().AddBBox( box, sf::Color::White );
        // TEST
    }
}


// Deactivate
//////////////////////////////////////////////////
void
grParticleSystemPB::Deactivate( grParticleSetupPB& rParticleSetup )
{
    vecParticle& rVec = rParticleSetup.VecParticle;
    sInt active = ( sInt )rParticleSetup.ParticlesActive;
    uInt nxtFree = active - 1;
    for ( sInt idx = 0; idx < active; ++idx )
    {
        if ( rVec[ idx ]->LifeTime < 0.0f )
        {
            grParticlePB& rToo = *rVec[ idx ].get();
            grParticlePB& rFrom = *rVec[ nxtFree ].get();

            rToo.Position = rFrom.Position;
            rToo.Velocity = rFrom.Velocity;
            rToo.Mass = rFrom.Mass;
            rToo.LifeTime = rFrom.LifeTime;

            // TODO: This block should not be needed but without it things blow up. Investigate more.
            rFrom.Position = grV2f();
            rFrom.Velocity = grV2f();
            rFrom.Mass = 0.0f;
            rFrom.LifeTime = 0.0f;

            --rParticleSetup.ParticlesActive;
            --idx;
        }
    }
}


// Activate
//////////////////////////////////////////////////
void
grParticleSystemPB::Activate( grParticleSetupPB& rParticleSetup, const float fixedT )
{
    rParticleSetup.SpawnCounter -= fixedT;
    if ( rParticleSetup.SpawnCounter < 0.0f )
    {
        rParticleSetup.SpawnCounter += rParticleSetup.SpawnInMilliSec;
        if ( rParticleSetup.ParticlesActive >= PARTICLE_PER_SYS )
            return;

        grParticlePB& rPart = *rParticleSetup.VecParticle[ rParticleSetup.ParticlesActive ].get();
        grParticleAttributePB& attribute = *rParticleSetup.pParticleAttribute;

        rPart.Position = attribute.Position;
        rPart.Acceleration = attribute.Acceleration;
        rPart.Mass = attribute.Mass;
        rPart.LifeTime = attribute.LifeTime;

        ++rParticleSetup.ParticlesActive;
    }
}
