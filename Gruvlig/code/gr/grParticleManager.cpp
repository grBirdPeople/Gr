#include	"grParticleManager.h"

#include	"grParticleSystem.h"
#include	"grDefine.h"


// cTor
//////////////////////////////////////////////////
grParticleManager::grParticleManager( void )
	: m_arrParticleSys	{}
	, m_SystemQuantity	( 0 )
{
	for ( uInt i = 0; i < MAX_PARTCILE_SYS; ++i )
	{
		m_arrParticleSys[ i ] = new grParticleSystem();
	}
}


// dTor
//////////////////////////////////////////////////
grParticleManager::~grParticleManager( void )
{
	for ( uInt i = 0; i < MAX_PARTCILE_SYS; ++i )
	{
		DELANDNULL( m_arrParticleSys[ i ] );
	}
}


// CreateParticleSystem
//////////////////////////////////////////////////
grParticleSystem* const
grParticleManager::CreateParticleSystem( void )
{
	if ( m_SystemQuantity == MAX_PARTCILE_SYS )
	{
#ifdef DEBUG
		std::cerr << "grParticleManager::CreateParticleSystem(): Max systems reached" << std::endl;
#endif // DEBUG
		return nullptr;
	}

	++m_SystemQuantity;
	return m_arrParticleSys[ m_SystemQuantity - 1 ];
}


// Update
//////////////////////////////////////////////////
void
grParticleManager::Update( const float deltaT )
{
	if ( m_SystemQuantity > 0 )
	{
		for ( uInt i = 0; i < m_SystemQuantity; ++i )
		{
			m_arrParticleSys[ i ]->Update( deltaT );
		}
	}
}
