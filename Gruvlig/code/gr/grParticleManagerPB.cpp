#include    "grParticleManagerPB.h"

#include    "grCore.h"
#include	"grDefine.h"


// cTor
//////////////////////////////////////////////////
grParticleManagerPB::grParticleManagerPB( void )
    : m_TimeStepCounter ( 0.0f )
    , m_SetupQuantity   ( 0 )
{}


// dTor
//////////////////////////////////////////////////
grParticleManagerPB::~grParticleManagerPB( void )
{
    for ( auto& i : m_vecParticleSetup )
        i.reset();

    for ( auto& i : m_vecSystems )
        i.reset();
}


// Init
//////////////////////////////////////////////////
void
grParticleManagerPB::Init( void )
{
    for ( uInt i = 0; i < PARTICLE_SYTEMS; ++i )
        m_vecSystems.push_back( std::move( std::make_unique<grParticleSystemPB>() ) );

    sInt id = -1;
    for ( uInt i = 0; i < PARTICLE_SETUPS; ++i )
    {
        std::unique_ptr<grParticleSetupPB> pSetup = std::make_unique<grParticleSetupPB>( ( uInt )++id );
        m_vecParticleSetup.push_back( std::move( pSetup ) );
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

    grParticleSetupPB* pSetup = m_vecParticleSetup[ m_SetupQuantity ].get();
    for ( uInt i = 0; i < PARTICLE_PER_SETUP; ++i )
        pSetup->vecParticle.push_back( std::move( std::make_unique<grParticlePB>() ) );

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
        for ( uInt idx = 0; idx < m_SetupQuantity; ++idx )
        {
            m_vecSystems[ 0 ]->Activate( *m_vecParticleSetup[ idx ], PARTICLE_TIMESTEP );
        }

        // Update
        for ( uInt idx = 0; idx < m_SetupQuantity; ++idx )
        {
            if ( m_vecParticleSetup[ idx ]->ParticlesActive == 0 )
                continue;

            m_vecSystems[ 0 ]->Update( *m_vecParticleSetup[ idx ], PARTICLE_TIMESTEP );
        }

        // Deactivate
        for ( uInt idx = 0; idx < m_SetupQuantity; ++idx )
        {
            if ( m_vecParticleSetup[ idx ]->ParticlesActive == 0 )
                continue;

            m_vecSystems[ 0 ]->Deactivate( *m_vecParticleSetup[ idx ] );
        }
        // TEST
    }
}