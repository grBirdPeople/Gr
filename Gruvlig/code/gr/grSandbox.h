#ifndef		_H_GRSANDBOX_
#define		_H_GRSANDBOX_

#include	<SFML/Graphics/RenderWindow.hpp>

#include	"grCommon.h"

class		grActor;
class		grMap;
class		grIEntity;
class		grInputManager;
class		grCParticleManager;
class		grCParticleEmitterPB;
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

	grCParticleEmitterPB*		m_pPartEmitter_1;
	grCParticleEmitterPB*		m_pPartEmitter_2;

	bool					m_bFlipFlop = true;

	float					m_LastMouseX,
							m_LastMouseY;

	sInt					m_PartSysIdOne,
							m_PartSysIdTwo;
};



#endif	// _H_GRSANDBOX_
