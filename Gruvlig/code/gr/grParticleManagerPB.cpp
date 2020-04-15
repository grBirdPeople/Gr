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
    : m_uPArrEmitters   ( new uP<SParticleEmitter>[ PARTICLE_EMITTERS ]() )
    , m_uPSystems       ( new uP<grParticleSystemPB>[ PARTICLE_SYTEMS ]() )
    , m_AttributeQue    ( new grStruct::grLoopQue<grParticlAttributePB>( PARTICLE_EMITTERS * PARTICLE_x_EMITTER ) )
    , m_CreatedSystems  ( 0 )
    , m_CreatedEmitters ( 0 )
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
        m_uPSystems[ i ] = std::make_unique<grParticleSystemPB>( ( uInt )i, PARTICLE_x_EMITTER );
        ++m_CreatedSystems;
    }
}


// dTor
//////////////////////////////////////////////////
grParticleManagerPB::~grParticleManagerPB( void )
{
    for ( sizeT i = 0; i < m_CreatedEmitters; ++i )
    {
        if ( m_uPArrEmitters[ i ] != nullptr )
            delete m_uPArrEmitters[ i ].release();
    }
    if ( m_uPArrEmitters != nullptr )
        delete m_uPArrEmitters.release();

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
    if ( m_CreatedEmitters >= PARTICLE_EMITTERS )
    {
#ifdef DEBUG
        std::puts( "grParticleManagerPB::CreateParticleSystem(): Max attributes already created\n" );
#endif // DEBUG
        return -1;
    }

    uInt id = m_CreatedEmitters;
    ++m_CreatedEmitters;
    m_TotalParticles += PARTICLE_x_EMITTER;
    m_uPArrEmitters[ id ] = std::make_unique<SParticleEmitter>( id, PARTICLE_x_EMITTER );
    return id;
}


// Get
//////////////////////////////////////////////////
grParticlAttributePB
grParticleManagerPB::Get( const uInt id )
{
    grParticlAttributePB part = *m_uPArrEmitters[ id ]->uPAttribute.get();
    return part;
}


// Set
//////////////////////////////////////////////////
void
grParticleManagerPB::Set( const sInt id, const grParticlAttributePB& rAtt )
{
    *m_uPArrEmitters[ id ]->uPAttribute.get() = rAtt;  // TODO: Que attributes and batch update all
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

    for ( sizeT i = 0; i < m_CreatedEmitters; ++i )
        m_uPSystems[ 0 ]->Activate( *m_uPArrEmitters[ i ].get(), deltaT );

    for ( sizeT i = 0; i < m_CreatedEmitters; ++i )
        m_uPSystems[ 0 ]->Update( *m_uPArrEmitters[ i ].get(), deltaT );

    for ( sizeT i = 0; i < m_CreatedEmitters; ++i )
        m_uPSystems[ 0 ]->Deactivate( *m_uPArrEmitters[ i ].get() );

    // TEST DRAW
    for ( sizeT i = 0; i < m_CreatedEmitters; ++i )
    {
        uInt size = m_uPArrEmitters[ i ]->PartActive;
        for ( sizeT j = 0; j < size; ++j )
        {
            // TEST
            grBBox box( grV2f( 20.0f, 20.0f ), m_uPArrEmitters[ i ]->uPArrParticle[ j ]->Position );
            grSColor color = m_uPArrEmitters[ i ]->uPArrParticle[ j ]->Color;
            sf::Color sfColor( color.R, color.G, color.B, color.A );
            grDebugManager::Instance().AddBBox( box, sfColor );
            // TEST
        }
    }
    // TEST DRAW
}