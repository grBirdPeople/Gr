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

	inline const grV2u GetWindowSize() const
	{
		return { ( intU )m_puVidMode->width, ( intU )m_puVidMode->height };
	}

	inline void SetWindowSize( const grV2u& widthHeight )
	{
		unsigned int w = ( unsigned int )widthHeight.x, h = ( unsigned int )widthHeight.y;

		if ( w == m_puVidMode->width && h == m_puVidMode->height )
			return;

		// If not fullscreen set new windowed size
		if ( w != m_puVidMode->getDesktopMode().width && h != m_puVidMode->getDesktopMode().height )
			m_WindowedSize = { ( intU )w, ( intU )h };

		if( w > m_puVidMode->getDesktopMode().width )
			w = m_puVidMode->getDesktopMode().width;

		if ( h > m_puVidMode->getDesktopMode().height )
			h = m_puVidMode->getDesktopMode().height;

		m_puVidMode->width = w;
		m_puVidMode->height = h;

		m_puRenderWin->create( *m_puVidMode, m_AppName, sf::Style::None, *m_puCSettings );
		m_puRenderWin->setVerticalSyncEnabled( m_VSync );
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
	inline void DeInitManager();
	inline void Input( grInputManager& rInputMan );
	inline void Update();
	inline void Render();
	inline void	Terminate();

	pU<sf::RenderWindow> m_puRenderWin;
	pU<sf::ContextSettings> m_puCSettings;
	pU<sf::Event> m_puEvent;
	pU<sf::VideoMode> m_puVidMode;
	pU<sf::Clock> m_puClock;
	str m_AppName;
	grV2u m_WindowedSize;
	float m_TotalElapsedT, m_Dt;
	intU m_FramesPerSec, m_Aa;
	bool m_VSync;

#ifdef DEBUG
	pU<grSandbox> m_puSandbox;
#endif // DEBUG
};

#endif		//_GRCORE_H_