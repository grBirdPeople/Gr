#include    "grParticleManagerPB.h"

#include    "grCore.h"
#include	"grDefine.h"


// cTor
//////////////////////////////////////////////////
grParticleManagerPB::grParticleManagerPB( void )
    : m_SetupQuantity   ( 0 )
    , m_TimeStepCounter ( 0.0f )
{}


// dTor
//////////////////////////////////////////////////
grParticleManagerPB::~grParticleManagerPB( void )
{
    for ( sizeT i = 0; i < PARTICLE_SETUPS; ++i )
        m_arrParticleSetup[ i ].release();

    for ( sizeT i = 0; i < PARTICLE_SYTEMS; ++i )
        m_arrSystems[ i ].release();    
}


// Init
//////////////////////////////////////////////////
void
grParticleManagerPB::Init( void )
{
    sInt id = -1;
    for ( sizeT i = 0; i < PARTICLE_SETUPS; ++i )
    {
        std::unique_ptr<grParticleSetupPB> pSetup = std::make_unique<grParticleSetupPB>( ( uInt )++id );
        m_arrParticleSetup[ i ] = std::move( pSetup );
    }

    for ( sizeT i = 0; i < PARTICLE_SYTEMS; ++i )
    {
        m_arrSystems[ i ] = std::make_unique<grParticleSystemPB>();
    }
}


// CreateParticleAttribute
//////////////////////////////////////////////////
inline grParticleAttributePB
grParticleManagerPB::CreateParticleAttribute( void )
{
    return grParticleAttributePB();
}


// CreateSystem
//////////////////////////////////////////////////
grParticleSetupPB* const
grParticleManagerPB::CreateSetup( void )
{
    if ( m_SetupQuantity >= PARTICLE_SETUPS )
    {
#ifdef DEBUG
        std::puts( "grParticleManagerPB::Create(): Max particle systems already created\n" );
#endif // DEBUG
        return nullptr;
    }

    grParticleSetupPB* pSetup = m_arrParticleSetup[ m_SetupQuantity ].get();
    for ( sizeT i = 0; i < PARTICLE_PER_SETUP; ++i )
        pSetup->arrParticle[ i ] = std::move( std::make_unique<grParticlePB>() );

    ++m_SetupQuantity;
    return pSetup;
}


// Update
//////////////////////////////////////////////////
void
grParticleManagerPB::Update( const float deltaT )
{
    m_TimeStepCounter -= deltaT;
    if ( m_TimeStepCounter < 0.0f )
    {
        // Reset time step
        m_TimeStepCounter += PARTICLE_TIMESTEP;

        // TEST
        // Activate
        for ( sizeT idx = 0; idx < m_SetupQuantity; ++idx )
        {
            m_arrSystems[ 0 ]->Activate( *m_arrParticleSetup[ idx ], PARTICLE_TIMESTEP );
        }

        // Update
        for ( sizeT idx = 0; idx < m_SetupQuantity; ++idx )
        {
            if ( m_arrParticleSetup[ idx ]->ParticlesActive == 0 )
                continue;

            m_arrSystems[ 0 ]->Update( *m_arrParticleSetup[ idx ], PARTICLE_TIMESTEP );
        }

        // Deactivate
        for ( sizeT idx = 0; idx < m_SetupQuantity; ++idx )
        {
            if ( m_arrParticleSetup[ idx ]->ParticlesActive == 0 )
                continue;

            m_arrSystems[ 0 ]->Deactivate( *m_arrParticleSetup[ idx ] );
        }
        // TEST
    }
}