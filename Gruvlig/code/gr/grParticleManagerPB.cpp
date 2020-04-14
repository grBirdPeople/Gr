#include    "grParticleManagerPB.h"

#include    "grCore.h"
#include	"grDefine.h"

// TEST DRAW
#include	"grBBox.h"
#include	"grDebugManager.h"
// TEST DRAW


// cTor
//////////////////////////////////////////////////
grParticleManagerPB::grParticleManagerPB( void )
    : m_uPArrPartBlocks ( new uPtr<SParticleBlock>[ PARTICLE_EMITTERS ]() )
    , m_uPSystems       ( new uPtr<grParticleSystemPB>[ PARTICLE_SYTEMS ]() )
    , m_AttributeQue    ( new grLoopQue<grParticlAttributePB>( PARTICLE_EMITTERS * PARTICLE_PER_ATTRIBUTE ) )
    , m_CreatedSystems  ( 0 )
    , m_CreatedBlocks   ( 0 )
    , m_TotalParticles  ( 0 )
{
    // I forget syntax dynamic-array-uPtr so example
    {
        // uPtr<grParticlePB*[]>	m_upArrParticle;
        // uPtr<uPtr<grParticlePB>[]>	m_upupArrParticle;

        // m_upArrParticle       ( new grParticlePB*[ PARTICLE_PER_ATTRIBUTE ]() )
        // m_upupArrParticle     ( new uPtr<grParticlePB>[ PARTICLE_PER_ATTRIBUTE ]() )
    }

    for ( sizeT i = 0; i < PARTICLE_SYTEMS; ++i )
    {
        m_uPSystems[ i ] = std::make_unique<grParticleSystemPB>( ( uInt )i );
        ++m_CreatedSystems;
    }
}


// dTor
//////////////////////////////////////////////////
grParticleManagerPB::~grParticleManagerPB( void )
{
    for ( sizeT i = 0; i < m_CreatedBlocks; ++i )
    {
        if ( m_uPArrPartBlocks[ i ] != nullptr )
            delete m_uPArrPartBlocks[ i ].release();
    }
    if ( m_uPArrPartBlocks != nullptr )
        delete m_uPArrPartBlocks.release();

    for ( sizeT i = 0; i < m_CreatedSystems; ++i )
    {
        if ( m_uPSystems[ i ] != nullptr )
            delete m_uPSystems[ i ].release();
    }
    if ( m_uPSystems != nullptr )
        delete m_uPSystems.release();
}


// CreateParticleSystem
//////////////////////////////////////////////////
const sInt
grParticleManagerPB::Create( void )
{
    if ( m_CreatedBlocks >= PARTICLE_EMITTERS )
    {
#ifdef DEBUG
        std::puts( "grParticleManagerPB::CreateParticleSystem(): Max attributes already created\n" );
#endif // DEBUG
        return -1;
    }

    uInt id = m_CreatedBlocks;
    ++m_CreatedBlocks;
    m_TotalParticles += PARTICLE_PER_ATTRIBUTE;
    m_uPArrPartBlocks[ id ] = std::make_unique<SParticleBlock>( id, PARTICLE_PER_ATTRIBUTE );
    return id;
}


// Get
//////////////////////////////////////////////////
grParticlAttributePB
grParticleManagerPB::Get( const uInt id )
{
    grParticlAttributePB part = *m_uPArrPartBlocks[ id ]->uPAttribute.get();
    return part;
}


// Set
//////////////////////////////////////////////////
void
grParticleManagerPB::Set( const sInt id, const grParticlAttributePB& rAtt )
{
    *m_uPArrPartBlocks[ id ]->uPAttribute.get() = rAtt;  // TODO: Que attributes and batch update all
}


// Update
//////////////////////////////////////////////////
void
grParticleManagerPB::Update( const float deltaT )
{
    // TEST

    // TODO: Test which loop-set is fastest

    //for ( sizeT i = 0; i < m_CreatedBlocks; ++i )
    //{
    //    m_uPSystems[ 0 ]->Activate( m_uPArrPartBlock[ i ], deltaT );
    //    m_uPSystems[ 0 ]->Update( m_uPArrPartBlock[ i ], deltaT );
    //    m_uPSystems[ 0 ]->Deactivate( m_uPArrPartBlock[ i ] );
    //}

    for ( sizeT i = 0; i < m_CreatedBlocks; ++i )
        m_uPSystems[ 0 ]->Activate( m_uPArrPartBlocks[ i ], deltaT );

    for ( sizeT i = 0; i < m_CreatedBlocks; ++i )
        m_uPSystems[ 0 ]->Update( m_uPArrPartBlocks[ i ], deltaT );

    for ( sizeT i = 0; i < m_CreatedBlocks; ++i )
        m_uPSystems[ 0 ]->Deactivate( m_uPArrPartBlocks[ i ] );


    // TEST DRAW
    for ( sizeT i = 0; i < m_CreatedBlocks; ++i )
    {
        uInt size = m_uPArrPartBlocks[ i ]->PartActive;
        for ( sizeT j = 0; j < size; ++j )
        {
            // TEST
            grBBox box( grV2f( 20.0f, 20.0f ), m_uPArrPartBlocks[ i ]->uPArrParticle[ j ]->Position );
            grParticleColor color = m_uPArrPartBlocks[ i ]->uPArrParticle[ j ]->Color;
            sf::Color sfColor( color.R, color.G, color.B, color.A );
            grDebugManager::Instance().AddBBox( box, sfColor );
            // TEST
        }
    }
    // TEST DRAW
}