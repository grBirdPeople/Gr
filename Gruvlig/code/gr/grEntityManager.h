#ifndef		_GRENTITYMANAGER_H_
#define		_GRENTITYMANAGER_H_

#define		MAX_ENEMY	32

#include	<SFML/Graphics/RenderWindow.hpp>

#include	"grIEntity.h"
#include	"grSingleton.h"

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
	
			grIEntity*	CreateEntity		( const grEnums::EntityType type, const str& rUniqueName, const grV2f& rPos );
			void		DestroyEntity		( grIEntity* pEntity );
			void		Update				( const float deltaT );
	
	//////////////////////////////////////////////////

private:
			void		Enable				( grIEntity* rEntity );
			void		Disable				( grIEntity* rEntity );
			grIEntity*	CreatePlayer		( const grEnums::EntityType type, const str& rUniqueName, const grV2f& rPos );
			grIEntity*	CreateEnemy			( const grEnums::EntityType type, const str& rUniqueName, const grV2f& rPos );
	inline	intU		CreateAndGetId		( void)	const																		{ intU id = m_Id;	++m_Id;	return id; }


	//////////////////////////////////////////////////
	
	grEntityEnemy*	m_pArrEnemy[ MAX_ENEMY ];
	intU			m_EnemiesActive;
	intU			m_EnemyQuantity;

	grEntityPlayer*	m_pPlayerEntity;
	
	//////////////////////////////////////////////////
	
	static	intU	m_Id;

};

#endif		//_GRENTITYMANAGER_H_
