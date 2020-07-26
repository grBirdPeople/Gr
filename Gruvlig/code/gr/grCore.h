#ifndef		_GRCORE_H_
#define		_GRCORE_H_

#define		AA_MAX	8	// SFML max

#include	<SFML/Graphics/RenderWindow.hpp>
#include	<SFML/Window/ContextSettings.hpp>
#include	<SFML/Window/Event.hpp>
#include	<SFML/System/Clock.hpp>

#include	"grCommon.h"
#include	"grSingleton.h"
#include	"grV2.h"

class		grSandbox;
class		grInputManager;


class grCore : public grSingleton<grCore>
{
public:
	grCore();
	~grCore();
	
	inline const float GetGameTimeElapsed() const
	{
		return m_TotalElapsedT;
	}

	inline const float GetDeltaT() const
	{
		return m_Dt;
	}

	inline sf::RenderWindow& GetRenderWin() const
	{
		return *m_puRenderWin;
	}

	inline void SetWindowSize( const intU width, const intU height )
	{
		if ( m_puRenderWin->getSize().x == ( unsigned int )width && m_puRenderWin->getSize().y == ( unsigned int )height )
			return;

		if( sf::VideoMode::getDesktopMode().width != ( unsigned int )width && sf::VideoMode::getDesktopMode().height != ( unsigned int )height )
			m_WinSize = { width, height };

		m_puRenderWin->create( sf::VideoMode( ( unsigned int )width, ( unsigned int )height ), m_AppName, sf::Style::None, *m_puCSettings );
	}

	inline const grV2u GetWindowSize() const
	{
		return m_WinSize;
	}

	inline void SetAA ( const intU aa )
	{
		m_Aa = aa;
		m_puCSettings->antialiasingLevel = ( unsigned int )m_Aa;
	}	

	inline void SetFramerateLimit( const intU frameRateLimit )
	{
		m_FramesPerSec = frameRateLimit;
		m_puRenderWin->setFramerateLimit( ( unsigned int )m_FramesPerSec );
	}

	inline void SetVSync( const bool vSync )
	{
		m_VSync = vSync;
		m_puRenderWin->setVerticalSyncEnabled( m_VSync );
	}

	void Init( const str& rAppName = "grFramework", const intU winWidth = 960, const intU winHeight = 540, const intU frameRate = 120, const bool vsync = false );
	void Run();
	
private:

	inline void InitRenderWin();
	inline void InitManager();
	inline void Input( grInputManager& rInputMan );
	inline void Update();
	inline void Render();
	inline void	Terminate();

	pU<sf::RenderWindow> m_puRenderWin;
	pU<sf::ContextSettings> m_puCSettings;
	pU<sf::Event> m_puEvent;
	pU<sf::Clock> m_puClock;
	str m_AppName;
	grV2u m_WinSize;
	float m_TotalElapsedT, m_Dt;
	intU m_FramesPerSec, m_Aa;
	bool m_VSync;

#ifdef DEBUG
	pU<grSandbox> m_puSandbox;
#endif // DEBUG
};

#endif		//_GRCORE_H_

