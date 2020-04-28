#ifndef		_H_GRSANDBOX_
#define		_H_GRSANDBOX_

#include	<SFML/Graphics/RenderWindow.hpp>

#include	"grCommon.h"
#include	"grV2.h"

class		grActor;
class		grMap;
class		grIEntity;
class		grInputManager;
class		grCParticleManager;
class		grCParticleEmitter;


// grSandbox
//////////////////////////////////////////////////
class grSandbox
{
public:

	grSandbox	( void );

	grSandbox( grSandbox const& ) = delete;
	grSandbox& operator=( grSandbox const& ) = delete;

	//////////////////////////////////////////////////

	void Update( const float deltaT );
	void Render( sf::RenderWindow& rRenderWin );

	//////////////////////////////////////////////////

private:

	grInputManager&			m_rInputMan;
	grCParticleManager&		m_rPartMan;

	grCParticleEmitter&		m_Emitr1;

	grMap*					m_pMap;
	grActor*				m_pActor;

	grIEntity*				m_pPlayer;
	grIEntity*				m_pEnemy;

	bool					m_bFlipFlop = true;

	grV2f					m_LastMousePos;

	intS					m_PartSysIdOne,
							m_PartSysIdTwo;
};



#endif	// _H_GRSANDBOX_
