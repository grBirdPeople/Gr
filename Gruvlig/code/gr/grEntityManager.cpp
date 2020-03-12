#include	"grEntityManager.h"

#include	"grDefine.h"
#include	"grEntityPlayer.h"
#include	"grIEntity.h"
#include	"grEntityEnemy.h"


// cTor
//////////////////////////////////////////////////
grEntityManager::grEntityManager( void )
	: m_EnemiesActive	( 0 )
	, m_EnemyQuantity	( 0 )
	, m_pArrEnemySorted	{}
	, m_pEntityHashMap	( new grHashMap<grIEntity*>( MAX_ENTITY ) )
	, m_pPlayerEntity	( nullptr )
{
	for( uInt i = 0; i < MAX_ENEMY; ++i )	// TODO: Create dummy instances and reuse them
	{
		m_pArrEnemySorted[ i ] = nullptr;
	}
}


// dTor
//////////////////////////////////////////////////
grEntityManager::~grEntityManager( void )
{
	DEL_NULL( m_pEntityHashMap );
	DEL_NULL( m_pPlayerEntity );

	for( uInt i = 0; i < m_EnemyQuantity; ++i )
	{
		DEL_NULL( m_pArrEnemySorted[ i ] );
	}
}


// CreateEntity
//////////////////////////////////////////////////
grIEntity*
grEntityManager::CreateEntity( const grIEntity::EEntityType type, const str& rUniqueName, const grV2f& rPos )
{
	switch (type)
	{
	case grIEntity::EEntityType::PLAYER:	return CreatePlayer( type, rUniqueName, rPos );	break;
	case grIEntity::EEntityType::ENEMY:		return CreateEnemy( type, rUniqueName, rPos );	break;
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
	grIEntity::EEntityType entityType = pEntity->GetType();
	uInt entityId = pEntity->GetId();
	switch ( entityType )
	{
	case grIEntity::EEntityType::ENEMY:
	{
		grEntityEnemy* pEnemy = (grEntityEnemy*)pEntity;
		for ( uInt i = 0; i < m_EnemyQuantity; ++i )
		{
			if ( m_pArrEnemySorted[ i ]->GetId() == entityId )	// TODO: Get from hash map instead
			{
				pEnemy = m_pArrEnemySorted[ i ];

				if ( pEnemy->GetEnable() == true )
				{
					m_pArrEnemySorted[ i ] = m_pArrEnemySorted[ m_EnemiesActive - 1 ];
					m_pArrEnemySorted[ m_EnemiesActive - 1 ] = m_pArrEnemySorted[ m_EnemyQuantity - 1 ];
				}
				else
				{
					m_pArrEnemySorted[ i ] = m_pArrEnemySorted[ m_EnemyQuantity - 1 ];
				}

				m_pArrEnemySorted[ m_EnemyQuantity - 1 ] = nullptr;
				m_pEntityHashMap->Del( entityId );
				--m_EnemiesActive;
				--m_EnemyQuantity;
				DEL_NULL( pEnemy );
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

	case grIEntity::EEntityType::PLAYER:
	{
		if ( m_pPlayerEntity != nullptr )
		{
			m_pEntityHashMap->Del( m_pPlayerEntity->GetId() );
			DEL_NULL( m_pPlayerEntity );
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

	for( uInt i = 0; i < m_EnemyQuantity; ++i )
	{
		if ( m_pArrEnemySorted[ i ] != nullptr )
		{
			if ( m_pArrEnemySorted[ i ]->GetEnable() == true )
			{
				m_pArrEnemySorted[ i ]->Update( deltaT );
			}
		}
	}
}


// Activate
//////////////////////////////////////////////////
void
grEntityManager::Enable( grIEntity* pEntity )
{
	uInt id = pEntity->GetId();
	for ( uInt i = m_EnemiesActive; i < m_EnemyQuantity; ++i )
	{
		if ( m_pArrEnemySorted[ i ]->GetId() == id )
		{
			grEntityEnemy* tmp = m_pArrEnemySorted[ i ];

			if ( i == m_EnemiesActive )
			{
				++m_EnemiesActive;
				return;
			}

			for ( uInt j = 0; j < m_EnemyQuantity; ++j )
			{
				m_pArrEnemySorted[ i ] = m_pArrEnemySorted[ j ];
				m_pArrEnemySorted[ j ] = tmp;

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
	uInt id = pEntity->GetId();
	for ( uInt i = 0; i < m_EnemiesActive; ++i )
	{
		if ( m_pArrEnemySorted[ i ]->GetId() == id )
		{
			grEntityEnemy* pEntity = m_pArrEnemySorted[ i ];

			if ( i == m_EnemiesActive )
			{
				--m_EnemiesActive;
				return;
			}

			m_pArrEnemySorted[ i ] = m_pArrEnemySorted[ m_EnemiesActive - 1 ];
			m_pArrEnemySorted[ m_EnemiesActive - 1 ] = pEntity;
			--m_EnemiesActive;
			return;
		}
	}
}


// CreatePlayer
//////////////////////////////////////////////////
grIEntity*
grEntityManager::CreatePlayer( const grIEntity::EEntityType type, const str& rUniqueName, const grV2f& rPos )
{
#ifdef DEBUG
	if( m_pPlayerEntity != nullptr )
	{
		std::cerr << "error: grEntityManager::CreatePlayer: EntityPlayer already created\n";
		return m_pPlayerEntity;
	}
#endif
	
	uInt id = CreateId();
	m_pPlayerEntity = new grEntityPlayer( type, rUniqueName, rPos, id );
	m_pEntityHashMap->Set( id, (grIEntity*)m_pPlayerEntity );
	return m_pPlayerEntity;
}


// CreateEnemy
//////////////////////////////////////////////////
grIEntity*
grEntityManager::CreateEnemy( const grIEntity::EEntityType type, const str& rUniqueName, const grV2f& rPos )
{
	if( m_EnemyQuantity == MAX_ENEMY )
	{
		std::cerr << "error: grEntityManager::CreateEnemy: Max EntityEnemy already created\n";
		return nullptr;
	}
	
	uInt id = CreateId();
	grEntityEnemy* pEnemy = new grEntityEnemy( type, rUniqueName, rPos, id );
	m_pArrEnemySorted[ m_EnemyQuantity ] = pEnemy;
	m_pEntityHashMap->Set( id, (grIEntity*)pEnemy );
	++m_EnemiesActive;
	++m_EnemyQuantity;
	
	return m_pArrEnemySorted[ m_EnemyQuantity - 1 ];
}


// Static
//////////////////////////////////////////////////
uInt grEntityManager::m_Id = 0;
