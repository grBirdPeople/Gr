#include	"grParticleSystemPB.h"

#include	"grBBox.h"
#include	"grDebugManager.h"
#include	"grParticleManagerPB.h"
#include    "grRandom.h"
#include	"grV2.h"


// cTor
//////////////////////////////////////////////////
grParticleSystemPB::grParticleSystemPB( void )
    :   pRandom ( new grRandom() )
{}


// dTor
//////////////////////////////////////////////////
grParticleSystemPB::~grParticleSystemPB( void )
{
    if( pRandom  != nullptr)
        DELANDNULL( pRandom );
}


// Activate
//////////////////////////////////////////////////
void
grParticleSystemPB::Activate( grParticleSetupPB& rParticleSetup, const float fixedT )
{
    grParticleAttributePB& rAttribute = *rParticleSetup.pAttribute;
    rAttribute.SpawnCounter -= fixedT;
    if ( rAttribute.SpawnCounter < 0.0f )
    {
        rAttribute.SpawnCounter += rAttribute.SpawnInMilliSec;
        if ( rParticleSetup.ParticlesActive >= PARTICLE_PER_SETUP )
            return;

        grParticlePB& rPart = *rParticleSetup.vecParticle[ rParticleSetup.ParticlesActive ].get();

        rPart.Position = rAttribute.Position;
        rPart.Acceleration = rAttribute.Acceleration;
        rPart.Mass = rAttribute.Mass;

        // Speed
        {

        }

        // Lifetime
        {
            rPart.LifeTime = ( rAttribute.bLifetimeRange == true )
                ? pRandom->GetRandFloat( rAttribute.LifetimeRange.x, rAttribute.LifetimeRange.y )
                : rAttribute.LifetimeRange.y;
        }



        ++rParticleSetup.ParticlesActive;
    }
}


// Update
//////////////////////////////////////////////////
void
grParticleSystemPB::Update( grParticleSetupPB& rParticleSetup, const float fixedT )
{
    vecParticle& rVec = rParticleSetup.vecParticle;
    grParticleAttributePB& rAttribute = *rParticleSetup.pAttribute.get();
    for ( uInt idx = 0; idx < rParticleSetup.ParticlesActive; ++idx )
    {
        grParticlePB& rPart = *rParticleSetup.vecParticle[ idx ].get();

        rPart.Acceleration += rAttribute.Gravity / rPart.Mass;

        rPart.Velocity += rPart.Acceleration * fixedT;
        rPart.Position += rPart.Velocity * fixedT;

        rPart.LifeTime -= fixedT;
        rPart.Acceleration = grV2f();

        // TEST
        //printf( "%s %d \n", "Particles Active: ", rParticleSetup.ParticlesActive );
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
    vecParticle& rVec = rParticleSetup.vecParticle;
    sInt active = ( sInt )rParticleSetup.ParticlesActive;
    for ( sInt idx = 0; idx < active; ++idx )
    {
        if ( rVec[ idx ]->LifeTime < 0.0f )
        {
            grParticlePB& rFrom = *rVec[ --active ].get();
            grParticlePB& rToo = *rVec[ idx ].get();

            rToo.Position = rFrom.Position;
            rToo.Velocity = rFrom.Velocity;
            rToo.Mass = rFrom.Mass;
            rToo.LifeTime = rFrom.LifeTime;

            // TODO: This reset block should not be needed but without it things blow up. Investigate more.
            // Hint: If idx equals active it writes to itself
            rFrom.Position = grV2f();
            rFrom.Velocity = grV2f();
            rFrom.Mass = 0.0f;
            rFrom.LifeTime = 0.0f;

            --idx;
        }
    }
    rParticleSetup.ParticlesActive = active;
}