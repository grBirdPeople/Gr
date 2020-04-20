#include	"grParticleSystemPB.h"

#include    "grRandom.h"
#include    "grParticleEmitterPB.h"
#include    "grAlgo.h"

// TEST DRAW
#include	"grBBox.h"
#include	"grDebugManager.h"
// TEST DRAW


// cTor
//////////////////////////////////////////////////
grSParticleSystemPB::grSParticleSystemPB( const uInt id, const uInt partQuantity )
    : uPRand                    ( new grRandom )
    , Id                        ( id )
{}


// dTor
//////////////////////////////////////////////////
grSParticleSystemPB::~grSParticleSystemPB( void )
{
    if( uPRand  != nullptr)
        delete uPRand.release();
}


// Activate
//////////////////////////////////////////////////
void
grSParticleSystemPB::Activate( grCParticleEmitterPB& rEmitter, const float deltaT )
{
    rEmitter.SpawnCounter -= deltaT;
    if ( rEmitter.SpawnCounter <= 0.0f )
    {
        rEmitter.SpawnCounter += rEmitter.SpawnInMilliSec;
        if ( rEmitter.PartActive >= rEmitter.PartSize )
            return;

        // Think writing data should maybe be done elsewhere
        {
            grSParticlePB& rPart = *rEmitter.vecUpParticles[ rEmitter.PartActive ].get();
            grCParticleAttributePB& rAtt = rEmitter.GetParticleAttribute();

            rPart.Color = rAtt.Color;
            rPart.Position = rAtt.Position;
            rPart.Velocity = rAtt.Velocity;
            rPart.Lifetime = rAtt.Lifetime;
        }

        ++rEmitter.PartActive;
    }


    {
        //grParticleAttributePB& rAttribute = *rParticleSetup.pAttribute;
        //rAttribute.SpawnCounter -= fixedT;
        //if ( rAttribute.SpawnCounter < 0.0f )
        //{
        //    rAttribute.SpawnCounter += rAttribute.SpawnInMilliSec;
        //    if ( rParticleSetup.ParticlesActive >= PARTICLE_PER_SETUP )
        //        return;

        //    grParticlePB& rPart = *rParticleSetup.arrParticle[ rParticleSetup.ParticlesActive ].get();

        //    rPart.Position = rAttribute.Position;
        //    rPart.Mass = rAttribute.Mass;


        //    // Speed
        //    float speed = 0.0f;
        //    if ( rAttribute.bSpeedRange == true )
        //    {
        //        speed = pRand->Float( rAttribute.SpeedRange.x, rAttribute.SpeedRange.y );
        //    }
        //    else
        //    {
        //        speed = rAttribute.SpeedRange.x;
        //    }

        //    // Direction
        //    grV2f dir = grV2f( 0.0f, -1.0f );
        //    if ( rAttribute.bDirectionRange == true )
        //    {
        //        grV2f range = rAttribute.DirectionRange;
        //        float randDeg = 0.0f;
        //        if ( range.x > range.y )
        //        {
        //            range.x = grMath::Abs( range.x - 360.0f );
        //            randDeg = range.x + range.y;
        //            randDeg = pRand->Float( 0.0f, randDeg );
        //            randDeg -= range.x;
        //        }
        //        else
        //        {
        //            randDeg = range.y - range.x;
        //            randDeg = pRand->Float( 0.0f, randDeg );
        //            randDeg += range.x;
        //        }

        //        grMath::RotatePoint( &dir, randDeg * grMath::DegToRad );
        //    }
        //    else
        //    {
        //        grMath::RotatePoint( &dir, rAttribute.DirectionRange.x * grMath::DegToRad );
        //    }

        //    // Lifetime
        //    if ( rAttribute.bLifetimeRange == true )
        //    {
        //        rPart.Lifetime = pRand->Float( rAttribute.LifetimeRange.x, rAttribute.LifetimeRange.y );
        //    }
        //    else
        //    {
        //        rPart.Lifetime = rAttribute.LifetimeRange.x;
        //    }

        //    // Accelerations
        //    rPart.Acceleration += ( dir * speed ) / rPart.Mass;


        //    ++rParticleSetup.ParticlesActive;
        //}
    }
}


// Update
//////////////////////////////////////////////////
void
grSParticleSystemPB::Update( grCParticleEmitterPB& rEmitter, const float deltaT )
{
    if ( rEmitter.PartActive > 0 )
    {
        for ( sizeT idx = 0; idx < rEmitter.PartActive; ++idx )
        {
            rEmitter.vecUpParticles[ idx ]->Lifetime -= deltaT;
            if ( rEmitter.vecUpParticles[ idx ]->Lifetime <= grMath::Epsilon )
            {
                rEmitter.uPPartDeactivateQue->Push( idx );
                continue;
            }

            rEmitter.vecUpParticles[ idx ]->Position += rEmitter.vecUpParticles[ idx ]->Velocity * deltaT;

            // TEST DRAW
            grBBox box( grV2f( 20.0f, 20.0f ), rEmitter.vecUpParticles[ idx ]->Position );
            grSColor color = rEmitter.vecUpParticles[ idx ]->Color;
            grDebugManager::Instance().AddBBox( box, sf::Color( color.R, color.G, color.B, color.A ) );
            // TEST DRAW
        }
    }






    {
        //grParticleAttributePB& rAttribute = *rParticleSetup.pAttribute.get();
        //for ( sizeT idx = 0; idx < rParticleSetup.ParticlesActive; ++idx )
        //{
        //    grParticlePB& rPart = *rParticleSetup.arrParticle[ idx ].get();


        //    if ( rAttribute.bSpeedOsc == true )
        //    {
        //        if ( rPart.Velocity.x != NULL && rPart.Velocity.y != NULL )
        //        {
        //            grV2f norm = rPart.Velocity.Normalized();
        //            rPart.Acceleration += ( norm * rAttribute.SpeedOsc ) / rPart.Mass;
        //        }
        //    }



        //    rPart.Acceleration += rAttribute.Gravity / rPart.Mass;

        //    rPart.Velocity += rPart.Acceleration * fixedT;
        //    rPart.Position += rPart.Velocity * fixedT;

        //    rPart.Lifetime -= fixedT;
        //    rPart.Acceleration = grV2f();

        //    // TEST
        //    //printf( "%s %d \n", "Particles Active: ", rParticleSetup.ParticlesActive );
        //    grBBox box( grV2f( 20.0f, 20.0f ), rPart.Position );
        //    grDebugManager::Instance().AddBBox( box, sf::Color::White );
        //    // TEST
        //}
    }
}


// Deactivate
//////////////////////////////////////////////////
void
grSParticleSystemPB::Deactivate( grCParticleEmitterPB& rEmitter )
{
    if ( rEmitter.uPPartDeactivateQue->Quantity() > 0 )
    {
        uInt size = rEmitter.uPPartDeactivateQue->Quantity();
        for ( uInt i = 0; i < size; ++i )
            rEmitter.uPArrPartDeactivateSortd[ i ] = rEmitter.uPPartDeactivateQue->Pull();
            //uPArrDeactivateIdSortd[ i ] = rEmitter.uPPartDeactivateQue->Pull();

        grAlgo::InsrtSort<uInt>( rEmitter.uPArrPartDeactivateSortd.get(), size );

        for ( uInt idx = 0; idx < size; ++idx )
        {
            grSParticlePB& rTooPart = *rEmitter.vecUpParticles[ rEmitter.uPArrPartDeactivateSortd[ idx ] ];
            grSParticlePB& rFromPart = *rEmitter.vecUpParticles[ rEmitter.PartActive - 1 ];

            rTooPart = rFromPart;   // TODO: Make sure this actually copies the values

            // Should not need this
            //rFromPart.Position = 0.0f;
            //rFromPart.Velocity = 0.0f;
            //rFromPart.Lifetime = 0.0f;

            --rEmitter.PartActive;
        }
    }



    

    {
        //sInt active = ( sInt )rParticleSetup.ParticlesActive;
        //for ( sInt idx = 0; idx < active; ++idx )
        //{
        //    if ( rParticleSetup.arrParticle[ idx ]->Lifetime < 0.0f )
        //    {
        //        grParticlePB& rFrom = *rParticleSetup.arrParticle[ --active ].get();
        //        grParticlePB& rToo = *rParticleSetup.arrParticle[ idx ].get();

        //        rToo.Position = rFrom.Position;
        //        rToo.Velocity = rFrom.Velocity;
        //        rToo.Mass = rFrom.Mass;
        //        rToo.Lifetime = rFrom.Lifetime;

        //        // TODO: This reset block should not be needed but without it things blow up. Investigate more.
        //        // Hint: If idx equals active it writes to itself
        //        rFrom.Position = grV2f();
        //        rFrom.Velocity = grV2f();
        //        rFrom.Mass = 0.0f;
        //        rFrom.Lifetime = 0.0f;

        //        --idx;
        //    }
        //}
        //rParticleSetup.ParticlesActive = active;
    }
}