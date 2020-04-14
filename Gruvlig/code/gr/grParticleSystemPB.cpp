#include	"grParticleSystemPB.h"

#include	"grBBox.h"
#include	"grDebugManager.h"
#include    "grRandom.h"
#include	"grV2.h"
#include    "grParticleManagerPB.h"


// cTor
//////////////////////////////////////////////////
grParticleSystemPB::grParticleSystemPB( const uInt id )
    : pRand ( new grRandom )
    , Id    ( id )
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
grParticleSystemPB::Activate( uPtr<SParticleBlock>& rPartBlock, const float deltaT )
{


        //rBlock->SpawnCounter -= deltaT;
        //if ( rBlock->SpawnCounter <= 0.0f )
        //{
        //    rBlock->SpawnCounter += rBlock->SpawnInMilliSec;
        //    if ( rBlock->ActiveParticles >= rBlock->Size )
        //        return;

        //    ++rBlock->ActiveParticles;

        //    // Think writing data should be done seperate
        //    {
        //        grParticlePB& rPart = *rBlock->uPArrParticles[ rBlock->ActiveParticles - 1 ];
        //        grParticlAttributePB& rAtt = *rBlock->uPAttribute.get();
        //        rPart.Position = rAtt.Position;
        //        rPart.Velocity = rAtt.Velocity;
        //    }
        //}



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


// Update
//////////////////////////////////////////////////
void
grParticleSystemPB::Update( uPtr<SParticleBlock>& rPartBlock, const float deltaT )
{
    //grParticlAttributePB& rAtt = *rBlock->uPAttribute.get();
    //for ( sizeT idx = 0; idx < rBlock->ActiveParticles; ++idx )
    //{
    //    rBlock->uPArrParticles[ idx ]->Lifetime -= deltaT;
    //    if ( rBlock->uPArrParticles[ idx ]->Lifetime < grMath::Epsilon )
    //    {
    //        rBlock->bitSet << idx;
    //        continue;
    //    }

    //    rBlock->uPArrParticles[ idx ]->Position += rBlock->uPArrParticles[ idx ]->Velocity * deltaT;
    //}

        //// TEST
        ////printf( "%s %d \n", "Particles Active: ", rParticleSetup.ParticlesActive );
        //grBBox box( grV2f( 20.0f, 20.0f ), rPart.Position );
        //grDebugManager::Instance().AddBBox( box, sf::Color::White );
        //// TEST




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


// Deactivate
//////////////////////////////////////////////////
void
grParticleSystemPB::Deactivate( uPtr<SParticleBlock>& rPartBlock )
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