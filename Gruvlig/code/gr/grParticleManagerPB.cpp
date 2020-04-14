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
    : m_uPArrPartBlock  ( new uPtr<SParticleBlock>[ PARTICLE_ATTRIBUTES ]() )
    , m_uPSystems       ( new uPtr<grParticleSystemPB>[ PARTICLE_SYTEMS ] )
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
        if ( m_uPArrPartBlock[ i ] != nullptr )
            delete m_uPArrPartBlock[ i ].release();
    }
    if ( m_uPArrPartBlock != nullptr )
        delete m_uPArrPartBlock.release();

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
grParticlAttributePB* const
grParticleManagerPB::CreateParticleSystem( void )
{
    if ( m_CreatedBlocks >= PARTICLE_ATTRIBUTES )
    {
#ifdef DEBUG
        std::puts( "grParticleManagerPB::Create(): Max particle setups already created\n" );
#endif // DEBUG
        return nullptr;
    }

    uInt id = m_CreatedBlocks;
    ++m_CreatedBlocks;
    m_TotalParticles += PARTICLE_PER_ATTRIBUTE;
    m_uPArrPartBlock[ id ] = std::make_unique<SParticleBlock>( id, PARTICLE_PER_ATTRIBUTE );
    return m_uPArrPartBlock[ id ]->uPAttribute.get();
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
        m_uPSystems[ 0 ]->Activate( m_uPArrPartBlock[ i ], deltaT );

    for ( sizeT i = 0; i < m_CreatedBlocks; ++i )
        m_uPSystems[ 0 ]->Update( m_uPArrPartBlock[ i ], deltaT );

    for ( sizeT i = 0; i < m_CreatedBlocks; ++i )
        m_uPSystems[ 0 ]->Deactivate( m_uPArrPartBlock[ i ] );


    // TEST DRAW
    for ( sizeT i = 0; i < m_CreatedBlocks; ++i )
    {
        uInt size = m_uPArrPartBlock[ i ]->PartActive;
        for ( sizeT j = 0; j < size; ++j )
        {
            // TEST
            grBBox box( grV2f( 20.0f, 20.0f ), m_uPArrPartBlock[ i ]->uPArrParticle[ j ]->Position );
            grDebugManager::Instance().AddBBox( box, sf::Color::White );
            // TEST
        }
    }
    // TEST DRAW
}