#include    "grParticleManagerPB.h"

#include    "grCore.h"
#include	"grDefine.h"
#include	"grParticleSystemPB.h"


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
    m_pParticleSystem.reset();

    for ( auto& i : m_VecParticleSetup )
    {
        i->pParticleAttribute.reset();
        for ( auto& p : i->VecParticle )
            p.reset();
    }
}


// Init
//////////////////////////////////////////////////
void
grParticleManagerPB::Init( void )
{
    sInt id = -1;
    m_VecParticleSetup.reserve( PARTICLE_SYS );
    for ( uInt i = 0; i < PARTICLE_SYS; ++i )
    {
        std::unique_ptr<grParticleSetupPB> pSetup = std::make_unique<grParticleSetupPB>();
        pSetup->Id = ( uInt )++id;
        pSetup->pParticleAttribute = std::make_unique<grParticleAttributePB>();
        m_VecParticleSetup.push_back( std::move( pSetup ) );
    }

    m_pParticleSystem = std::make_unique<grParticleSystemPB>();
}


// CreateSystem
//////////////////////////////////////////////////
grParticleSetupPB* const
grParticleManagerPB::Create( void )
{
    if ( m_SetupQuantity >= PARTICLE_SYS )
    {
#ifdef DEBUG
        std::puts( "grParticleManagerPB::Create(): Max particle systems already created\n" );
#endif // DEBUG
        return nullptr;
    }

    grParticleSetupPB* pSetup = m_VecParticleSetup[ m_SetupQuantity ].get();
    for ( uInt i = 0; i < PARTICLE_PER_SYS; ++i )
    {
        std::unique_ptr<grParticlePB> pPart = std::make_unique<grParticlePB>();
        pSetup->VecParticle.push_back( std::move( pPart ) );
    }

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

        // Update
        for ( uInt idx = 0; idx < m_SetupQuantity; ++idx )
        {
            if ( m_VecParticleSetup[ idx ]->ParticlesActive == 0 )
                continue;

            m_pParticleSystem->Update( *m_VecParticleSetup[ idx ], PARTICLE_TIMESTEP );
        }

        // Deactivate
        for ( uInt idx = 0; idx < m_SetupQuantity; ++idx )
        {
            if ( m_VecParticleSetup[ idx ]->ParticlesActive == 0 )
                continue;

            m_pParticleSystem->Deactivate( *m_VecParticleSetup[ idx ] );
        }

        // Activate
        for ( uInt idx = 0; idx < m_SetupQuantity; ++idx )
        {
            m_pParticleSystem->Activate( *m_VecParticleSetup[ idx ], PARTICLE_TIMESTEP );
        }
    }
}