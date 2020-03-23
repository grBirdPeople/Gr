#ifndef		_H_GRSANDBOX_
#define		_H_GRSANDBOX_

#include	<SFML/Graphics/RenderWindow.hpp>

class		grActor;
class		grMap;
class		grIEntity;
class		grParticleSystem;


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

	grMap* m_pMap;
	grActor* m_pActor;

	grIEntity* m_pPlayer;
	grIEntity* m_pEnemy;
	bool m_bFlipFlop = true;

	grParticleSystem* m_pParticleSys;

};

#endif	// _H_GRSANDBOX_
