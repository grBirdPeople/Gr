#ifndef		_H_GRSANDBOX_
#define		_H_GRSANDBOX_

#include	<SFML/Graphics/RenderWindow.hpp>

class		grActor;
class		grMap;
class		grIEntity;
class		grInputManager;
class		grParticleManagerPB;


// grSandbox
//////////////////////////////////////////////////
class grSandbox
{
public:

			grSandbox	( void );

	//////////////////////////////////////////////////

	void	Update		( const float deltaT );
	void	Render		( sf::RenderWindow& rRenderWin );

	//////////////////////////////////////////////////

private:

	grInputManager&			m_rInputMan;
	grParticleManagerPB&	m_rParticleManPB;

	grMap*					m_pMap;
	grActor*				m_pActor;

	grIEntity*				m_pPlayer;
	grIEntity*				m_pEnemy;

	bool					m_bFlipFlop = true;

};

#endif	// _H_GRSANDBOX_
