#include	"grEntityManager.h"

#include	"grEntityPlayer.h"
#include	"grIEntity.h"
#include	"grEntityEnemy.h"


// cTor
//////////////////////////////////////////////////
grEntityManager::grEntityManager( void )
	: m_pArrEnemy		{}
	, m_EnemiesActive	( 0 )
	, m_EnemyQuantity	( 0 )
	, m_pPlayerEntity	( nullptr )
{
	for( intU i = 0; i < MAX_ENEMY; ++i )
	{
		m_pArrEnemy[ i ] = nullptr;
	}
}


// dTor
//////////////////////////////////////////////////
grEntityManager::~grEntityManager( void )
{
	for( intU i = 0; i < m_EnemyQuantity; ++i )
	{
		DELANDNULL( m_pArrEnemy[ i ] );
	}

	DELANDNULL( m_pPlayerEntity );
}


// CreateEntity
//////////////////////////////////////////////////
grIEntity*
grEntityManager::CreateEntity( const grEnums::EntityType type, const str& rUniqueName, const grV2f& rPos )
{
	switch (type)
	{
	case grEnums::EntityType::PLAYER: return CreatePlayer( type, rUniqueName, rPos ); break;
	case grEnums::EntityType::ENEMY: return CreateEnemy( type, rUniqueName, rPos ); break;
	default: std::cerr << "grEntityManager::CreateEntity(): Something broke" << std::endl;
	}

#ifdef DEBUG
	std::cerr << "grEntityManager::CreateEntity(): Something broke" << std::endl;
#endif

	return nullptr;
}


// DestroyEntity
//////////////////////////////////////////////////
void
grEntityManager::DestroyEntity( grIEntity* pEntity )
{
	grEnums::EntityType entityType = pEntity->GetType();
	intU entityId = pEntity->GetId();

	switch ( entityType )
	{
	case grEnums::EntityType::ENEMY:
	{
		grEntityEnemy* pEnemy = (grEntityEnemy*)pEntity;
		for ( intU i = 0; i < m_EnemyQuantity; ++i )
		{
			if ( m_pArrEnemy[ i ]->GetId() == entityId )
			{
				pEnemy = m_pArrEnemy[ i ];

				if ( pEnemy->GetEnable() == true )
				{
					m_pArrEnemy[ i ] = m_pArrEnemy[ m_EnemiesActive - 1 ];
					m_pArrEnemy[ m_EnemiesActive - 1 ] = m_pArrEnemy[ m_EnemyQuantity - 1 ];
				}
				else
				{
					m_pArrEnemy[ i ] = m_pArrEnemy[ m_EnemyQuantity - 1 ];
				}

				m_pArrEnemy[ m_EnemyQuantity - 1 ] = nullptr;
				--m_EnemiesActive;
				--m_EnemyQuantity;
				DELANDNULL( pEnemy );
			}
#ifdef DEBUG
			else
			{
				std::cerr << "grEntityManager::DestroyEntity(): Entity does not exists" << std::endl;
			}
#endif
		}

	}
	break;

	case grEnums::EntityType::PLAYER:
	{
		if ( m_pPlayerEntity != nullptr )
		{
			DELANDNULL( m_pPlayerEntity );
		}
#ifdef DEBUG
		else
		{
			std::cerr << "grEntityManager::DestroyEntity(): Player does not exists" << std::endl;
		}
#endif
	}
		break;
	}
}


// Update
//////////////////////////////////////////////////
void
grEntityManager::Update( const float deltaT )
{
	if ( m_pPlayerEntity != nullptr )
	{
		if ( m_pPlayerEntity->GetEnable() == true )
		{
			m_pPlayerEntity->Update( deltaT );
		}
	}

	for( intU i = 0; i < m_EnemyQuantity; ++i )
	{
		if ( m_pArrEnemy[ i ] != nullptr )
		{
			if ( m_pArrEnemy[ i ]->GetEnable() == true )
			{
				m_pArrEnemy[ i ]->Update( deltaT );
			}
		}
	}
}


// Activate
//////////////////////////////////////////////////
void
grEntityManager::Enable( grIEntity* pEntity )
{
	intU id = pEntity->GetId();
	for ( intU i = m_EnemiesActive; i < m_EnemyQuantity; ++i )
	{
		if ( m_pArrEnemy[ i ]->GetId() == id )
		{
			grEntityEnemy* tmp = m_pArrEnemy[ i ];

			if ( i == m_EnemiesActive )
			{
				++m_EnemiesActive;
				return;
			}

			for ( intU j = 0; j < m_EnemyQuantity; ++j )
			{
				m_pArrEnemy[ i ] = m_pArrEnemy[ j ];
				m_pArrEnemy[ j ] = tmp;

				++m_EnemiesActive;
				return;
			}
		}
	}
}


// Deactivate
//////////////////////////////////////////////////
void
grEntityManager::Disable( grIEntity* pEntity )
{
	intU id = pEntity->GetId();
	for ( intU i = 0; i < m_EnemiesActive; ++i )
	{
		if ( m_pArrEnemy[ i ]->GetId() == id )
		{
			grEntityEnemy* tmp = m_pArrEnemy[ i ];

			if ( i == m_EnemiesActive )
			{
				--m_EnemiesActive;
				return;
			}

			m_pArrEnemy[ i ] = m_pArrEnemy[ m_EnemiesActive - 1 ];
			m_pArrEnemy[ m_EnemiesActive - 1 ] = tmp;
			--m_EnemiesActive;
			return;
		}
	}
}


// CreatePlayer
//////////////////////////////////////////////////
grIEntity*
grEntityManager::CreatePlayer( const grEnums::EntityType type, const str& rUniqueName, const grV2f& rPos )
{
#ifdef DEBUG
	if( m_pPlayerEntity != nullptr )
	{
		std::cerr << "error: grEntityManager::CreatePlayer: EntityPlayer already created\n";
		return m_pPlayerEntity;
	}
#endif
	
	m_pPlayerEntity = new grEntityPlayer( type, rUniqueName, rPos, CreateAndGetId() );
	return m_pPlayerEntity;
}


// CreateEnemy
//////////////////////////////////////////////////
grIEntity*
grEntityManager::CreateEnemy( const grEnums::EntityType type, const str& rUniqueName, const grV2f& rPos )
{
	if( m_EnemyQuantity == MAX_ENEMY )
	{
		std::cerr << "error: grEntityManager::CreateEnemy: Max EntityEnemy already created\n";
		return nullptr;
	}
	
	m_pArrEnemy[ m_EnemyQuantity ] = new grEntityEnemy( type, rUniqueName, rPos, CreateAndGetId() );
	++m_EnemiesActive;
	++m_EnemyQuantity;
	
	return m_pArrEnemy[ m_EnemyQuantity - 1 ];
}


// Static
//////////////////////////////////////////////////
intU grEntityManager::m_Id = 0;
