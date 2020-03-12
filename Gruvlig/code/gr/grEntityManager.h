#ifndef		_GRENTITYMANAGER_H_
#define		_GRENTITYMANAGER_H_

#define		MAX_ENTITY	64
#define		MAX_ENEMY	32

#include	<SFML/Graphics/RenderWindow.hpp>

#include	"grCommon.h"
#include	"grIEntity.h"
#include	"grSingleton.h"
#include	"grHashMap.h"

class		grEntityEnemy;
class		grEntityPlayer;


// grEntityManager
//////////////////////////////////////////////////
class grEntityManager : public grSingleton<grEntityManager>
{
public:

	friend class grIEntity;

	//////////////////////////////////////////////////
	
						grEntityManager		( void );
						~grEntityManager	( void );
	
	//////////////////////////////////////////////////
	
			grIEntity*	CreateEntity		( const grIEntity::EEntityType type, const str& rUniqueName, const grV2f& rPos );
			void		DestroyEntity		( grIEntity* pEntity );
			void		Update				( const float deltaT );
	
	//////////////////////////////////////////////////

private:
			void		Enable				( grIEntity* rEntity );
			void		Disable				( grIEntity* rEntity );
			grIEntity*	CreatePlayer		( const grIEntity::EEntityType type, const str& rUniqueName, const grV2f& rPos );
			grIEntity*	CreateEnemy			( const grIEntity::EEntityType type, const str& rUniqueName, const grV2f& rPos );
	inline	uInt		CreateId			( void)	const																		{ uInt id = m_Id;	++m_Id;	return id; }


	//////////////////////////////////////////////////

	uInt					m_EnemiesActive;
	uInt					m_EnemyQuantity;
	
	grEntityEnemy*			m_pArrEnemySorted[ MAX_ENEMY ];	// TODO: Make this grIEntity instead
	grHashMap<grIEntity*>*	m_pEntityHashMap;				// TODO: Has no use atm. Would be nice if it's possible to implement for Enable/Disable
	grEntityPlayer*			m_pPlayerEntity;
	
	static	uInt			m_Id;	// TODO: Generate keys in cTor and keep them sorted or in a hashmap so we can resuse previously used id's from destroyed entities. And why is this static?

};

#endif		//_GRENTITYMANAGER_H_
