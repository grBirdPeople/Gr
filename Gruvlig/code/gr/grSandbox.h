#ifndef		_H_GRSANDBOX_
#define		_H_GRSANDBOX_

#include	<SFML/Graphics/RenderWindow.hpp>

class		grActor;
class		grMap;
class		grIEntity;
class		grInputManager;
struct		grParticlAttributePB;


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
	grParticlAttributePB&	m_rParticleAttribute;

	grMap*					m_pMap;
	grActor*				m_pActor;

	grIEntity*				m_pPlayer;
	grIEntity*				m_pEnemy;

	bool					m_bFlipFlop = true;

};

#endif	// _H_GRSANDBOX_
