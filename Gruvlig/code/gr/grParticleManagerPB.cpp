#include    "grParticleManagerPB.h"

#include	"grDefine.h"


// cTor
//////////////////////////////////////////////////
grParticleManagerPB::grParticleManagerPB( void )
    : m_SystemQuantity ( 0 )
    , m_VecParticles ( vecParticles( PARTICLES_PER_SYS ) )
{
    for ( uInt i = 0; i < MAX_PARTICLE_SYS; ++i )
    {
        for ( uInt j = 0; j < PARTICLES_PER_SYS; ++j )
        {
            m_VecParticles[ i ].push_back( new grParticlePB() );
        }
    }

    m_VecId.reserve( MAX_PARTICLE_SYS );
    m_VecSystems.reserve( MAX_PARTICLE_SYS );
}


// dTor
//////////////////////////////////////////////////
grParticleManagerPB::~grParticleManagerPB( void )
{
    for ( auto& i : m_VecSystems )
    {
        if ( i != nullptr )
        {
            DELANDNULL( i );
        }
    }

    for ( auto& i : m_VecParticles )
    {
        for ( auto& j : i )
        {
            if ( j != nullptr )
            {
                DELANDNULL( j );
            }
        }
    }
}


// SetSystemQantity
//////////////////////////////////////////////////
void
grParticleManagerPB::SetSystemQantity( const uInt quantity )
{
    m_SystemQuantity = quantity;
}


// CreateSystem
//////////////////////////////////////////////////
grParticleSystemPB* const
grParticleManagerPB::CreateSystem( void )
{
    if ( m_SystemQuantity == MAX_PARTICLE_SYS )
    {
#ifdef DEBUG
        std::cerr << "grParticleManagerPB::CreateSystem(): Max systems allready in use. Increase max systems or use less systems" << std::endl;
#endif // DEBUG
        return nullptr;
    }

    ++m_SystemQuantity;

    int id = ( m_VecId.size() > 0 ) ? ++m_VecId[ m_VecId.size() - 1 ] : 0;
    grParticleSystemPB* pSys = new grParticleSystemPB( id );

    m_VecId.push_back( id );
    m_VecSystems.push_back( pSys );

    return pSys;
}


// Update
//////////////////////////////////////////////////
void
grParticleManagerPB::Update( const float deltaT )
{
    for ( uInt i = 0; i < m_SystemQuantity; ++i )
    {
        m_VecParticles;
    }



    //grV2f tmpForce = grV2f( 0.0f, -50.0f ) / rParticle.Mass;

    //float mass = rParticle.Mass;
    //rParticle.Acceleration += tmpForce / mass;
    //rParticle.Acceleration += m_Gravity / mass;

    //rParticle.Velocity += rParticle.Acceleration;
    //rParticle.Position += rParticle.Velocity;

    //// Reset
    //rParticle.Acceleration = grV2f();
}
