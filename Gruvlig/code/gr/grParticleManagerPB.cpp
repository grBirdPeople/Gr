#include    "grParticleManagerPB.h"

#include    "grCore.h"
#include	"grDefine.h"


// cTor
//////////////////////////////////////////////////
grParticleManagerPB::grParticleManagerPB( void )
    : m_uPtrArrAttPartCombo  ( new uPtr<AttPartCombo>[ PARTICLE_ATTRIBUTES ]() )
    , m_uPtrSystems         ( new uPtr<grParticleSystemPB>[ PARTICLE_SYTEMS ] )
    , m_CreatedSystems      ( 0 )
    , m_CreatedArrCombo     ( 0 )
    , m_TotalParticles      ( 0 )

    , m_Que                 ( new grDALQue<uInt>( PARTICLE_ATTRIBUTES ) )

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
        m_uPtrSystems[ i ] = std::make_unique<grParticleSystemPB>( ( uInt )i );
        ++m_CreatedSystems;
    }


    //m_Que = new DALQue<uInt>( PARTICLE_ATTRIBUTES );
    //for ( uInt i = 0; i < PARTICLE_ATTRIBUTES; ++i )
    //{
    //    m_Que->Push( i );
    //}
    //for ( uInt i = 0; i < PARTICLE_ATTRIBUTES; ++i )
    //{
    //    int num = m_Que->Pull();

    //    int f = 0;
    //}
}


// dTor
//////////////////////////////////////////////////
grParticleManagerPB::~grParticleManagerPB( void )
{
    for ( sizeT i = 0; i < m_CreatedSystems; ++i )
    {
        if ( m_uPtrSystems[ i ] != nullptr )
            m_uPtrSystems[ i ].release();
    }
    if ( m_uPtrSystems != nullptr )
        m_uPtrSystems.release();
}


// CreateParticleSystem
//////////////////////////////////////////////////
grParticleAttributePB* const
grParticleManagerPB::CreateParticleSystem( void )
{
    if ( m_CreatedArrCombo >= PARTICLE_ATTRIBUTES )
    {
#ifdef DEBUG
        std::puts( "grParticleManagerPB::Create(): Max particle setups already created\n" );
#endif // DEBUG
        return nullptr;
    }

    uInt id = m_CreatedArrCombo;
    ++m_CreatedArrCombo;
    m_TotalParticles += PARTICLE_PER_ATTRIBUTE;
    m_uPtrArrAttPartCombo[ id ] = std::make_unique<AttPartCombo>( id, PARTICLE_PER_ATTRIBUTE );
    return m_uPtrArrAttPartCombo[ id ]->uPtrAttribute.get();
}


// Update
//////////////////////////////////////////////////
void
grParticleManagerPB::Update( const float deltaT )
{
    // TEST
    for ( sizeT i = 0; i < m_CreatedArrCombo; ++i )
    {
            m_uPtrSystems[ 0 ]->Activate( m_uPtrArrAttPartCombo[ i ]->uPtrArrParticles,
                                    m_uPtrArrAttPartCombo[ i ]->uPtrAttribute,
                                    deltaT );

            m_uPtrSystems[ 0 ]->Update( m_uPtrArrAttPartCombo[ i ]->uPtrArrParticles,
                                m_uPtrArrAttPartCombo[ i ]->uPtrAttribute,
                                deltaT );

            m_uPtrSystems[ 0 ]->Deactivate( m_uPtrArrAttPartCombo[ i ]->uPtrArrParticles,
                                    m_uPtrArrAttPartCombo[ i ]->uPtrAttribute );
    }

    //for ( sizeT i = 0; i < m_CreatedArrCombo; ++i )
    //{
    //    m_uPtrSystems[ 0 ]->Activate( m_uPtrArrAttPartCombo[ i ]->uPtrArrParticles,
    //                                    m_uPtrArrAttPartCombo[ i ]->uPtrAttribute,
    //                                    deltaT );
    //}
    //for ( sizeT i = 0; i < m_CreatedArrCombo; ++i )
    //{
    //    m_uPtrSystems[ 0 ]->Update( m_uPtrArrAttPartCombo[ i ]->uPtrArrParticles,
    //                                m_uPtrArrAttPartCombo[ i ]->uPtrAttribute,
    //                                deltaT );
    //}
    //for ( sizeT i = 0; i < m_CreatedArrCombo; ++i )
    //{
    //    m_uPtrSystems[ 0 ]->Deactivate( m_uPtrArrAttPartCombo[ i ]->uPtrArrParticles,
    //                                    m_uPtrArrAttPartCombo[ i ]->uPtrAttribute );
    //}
}