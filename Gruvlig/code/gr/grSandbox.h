#ifndef		_H_GRSANDBOX_
#define		_H_GRSANDBOX_

#include	<SFML/Graphics/RenderWindow.hpp>

class		grActor;
class		grMap;
class		grIEntity;
class		grInput;
class		grParticleManager;


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

	grInput&			m_rInputMan;
	grParticleManager&	m_rParticleMan;

	grMap*				m_pMap;
	grActor*			m_pActor;

	grIEntity*			m_pPlayer;
	grIEntity*			m_pEnemy;

	bool				m_bFlipFlop = true;

};

#endif	// _H_GRSANDBOX_
