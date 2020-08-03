#ifndef _H_GRSANDBOX_
#define _H_GRSANDBOX_

#include <SFML/Graphics/RenderWindow.hpp>

#include "grCommon.h"
#include "grV2.h"

class grActor;
class grMap;
class grIEntity;
class grInputManager;
class grRandom;
class grCParticleSys;
class grBoidSys;

namespace sf
{
	class RenderWindow;
}


// grSandbox
//////////////////////////////////////////////////
class grSandbox
{
public:
	grSandbox	( void );

	grSandbox( grSandbox const& ) = delete;
	grSandbox& operator=( grSandbox const& ) = delete;
	grSandbox( grSandbox&& ) noexcept = delete;
	grSandbox operator=( grSandbox&& ) noexcept = delete;

	void Update( const float deltaT );
	void Render( sf::RenderWindow& rRenderWin );

private:
	sf::RenderWindow& m_RendWin;
	grInputManager& m_rInputMan;
	grCParticleSys* m_pPartSys1;
	grBoidSys* m_pBoidSys;
	grMap* m_pMap;
	grActor* m_pActor;
	grIEntity* m_pPlayer;
	grIEntity* m_pEnemy;
	bool m_bFlipFlop = true;
	grV2f m_LastMousePos;
	float m_ParticleAnimT = 0.25f, m_ParticleAnimCounter = 0.0f;
	grRandom* m_Rand;
};



#endif	// _H_GRSANDBOX_
