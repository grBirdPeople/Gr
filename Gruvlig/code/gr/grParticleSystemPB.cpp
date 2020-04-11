#include	"grParticleSystemPB.h"

#include	"grBBox.h"
#include	"grDebugManager.h"
#include	"grParticleManagerPB.h"
#include    "grRandom.h"
#include	"grV2.h"


// cTor
//////////////////////////////////////////////////
grParticleSystemPB::grParticleSystemPB( void )
    : pRand   ( std::make_unique<grRandom>() )
{}


// dTor
//////////////////////////////////////////////////
grParticleSystemPB::~grParticleSystemPB( void )
{
    if( pRand  != nullptr)
        pRand.reset();
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

        grParticlePB& rPart = *rParticleSetup.arrParticle[ rParticleSetup.ParticlesActive ].get();

        rPart.Position = rAttribute.Position;
        rPart.Acceleration = rAttribute.Acceleration;
        rPart.Mass = rAttribute.Mass;


        // Speed
        float speed = ( rAttribute.bSpeedRange == true )
            ? pRand->Float( rAttribute.SpeedRange.x, rAttribute.SpeedRange.y )
            : rAttribute.SpeedRange.x;

        // Direction
        grV2f dir = grV2f( 0.0f, -1.0f );
        {
            grV2f ran;
            ran.x = ( rAttribute.DirectionRange.x < 0.0f )
                ? pRand->Float( 360.0f + rAttribute.DirectionRange.x, 359.9f )
                : pRand->Float( 0.0f, rAttribute.DirectionRange.x );
            ran.y = ( rAttribute.DirectionRange.y < 0.0f )
                ? pRand->Float( 360.0f + rAttribute.DirectionRange.y, 359.9f )
                : pRand->Float( 0.0f, rAttribute.DirectionRange.y );

            ( pRand->IntU( 1 ) == 0 ) ? grMath::RotatePoint( &dir, ran.x * grMath::DegToRad ) : grMath::RotatePoint( &dir, ran.y * grMath::DegToRad );

            int j = 0;
        }

        //float deg = ( rAttribute.bDirectionRange == true )
        //    ? pRand->Float( rAttribute.DirectionRange.x, rAttribute.DirectionRange.y )
        //    : rAttribute.DirectionRange.x;
        //grMath::RotatePoint( &dir, deg * grMath::DegToRad );

        // TEST
        rPart.Acceleration += ( dir * speed ) / rPart.Mass;
        // TEST

        // Lifetime
        {
            rPart.Lifetime = ( rAttribute.bLifetimeRange == true )
                ? pRand->Float( rAttribute.LifetimeRange.x, rAttribute.LifetimeRange.y )
                : rAttribute.LifetimeRange.x;
        }



        ++rParticleSetup.ParticlesActive;
    }
}


// Update
//////////////////////////////////////////////////
void
grParticleSystemPB::Update( grParticleSetupPB& rParticleSetup, const float fixedT )
{
    grParticleAttributePB& rAttribute = *rParticleSetup.pAttribute.get();
    for ( sizeT idx = 0; idx < rParticleSetup.ParticlesActive; ++idx )
    {
        grParticlePB& rPart = *rParticleSetup.arrParticle[ idx ].get();

        rPart.Acceleration += rAttribute.Gravity / rPart.Mass;

        rPart.Velocity += rPart.Acceleration * fixedT;
        rPart.Position += rPart.Velocity * fixedT;

        rPart.Lifetime -= fixedT;
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
    sInt active = ( sInt )rParticleSetup.ParticlesActive;
    for ( sInt idx = 0; idx < active; ++idx )
    {
        if ( rParticleSetup.arrParticle[ idx ]->Lifetime < 0.0f )
        {
            grParticlePB& rFrom = *rParticleSetup.arrParticle[ --active ].get();
            grParticlePB& rToo = *rParticleSetup.arrParticle[ idx ].get();

            rToo.Position = rFrom.Position;
            rToo.Velocity = rFrom.Velocity;
            rToo.Mass = rFrom.Mass;
            rToo.Lifetime = rFrom.Lifetime;

            // TODO: This reset block should not be needed but without it things blow up. Investigate more.
            // Hint: If idx equals active it writes to itself
            rFrom.Position = grV2f();
            rFrom.Velocity = grV2f();
            rFrom.Mass = 0.0f;
            rFrom.Lifetime = 0.0f;

            --idx;
        }
    }
    rParticleSetup.ParticlesActive = active;
}