#ifndef		_GRCORE_H_
#define		_GRCORE_H_

#define		AA_DEFAULT	8	// 8 is max for SFML

#include	<SFML/Graphics/RenderWindow.hpp>
#include	<SFML/Window/ContextSettings.hpp>
#include	<SFML/Window/Event.hpp>
#include	<SFML/System/Clock.hpp>

#include	"grCommon.h"
#include	"grSingleton.h"

class		grSandbox;


// grCore
//////////////////////////////////////////////////
class grCore : public grSingleton<grCore>
{
public:
	grCore( const intU winWidth = 640, const intU winHeight = 360, const intU frameRate = 120, const str& rAppName = "grFramework" );
	//grCore( const intU winWidth = 1920, const intU winHeight = 1080, const intU frameRate = 60, const str& rAppName = "grFramework" );
	~grCore();

	//////////////////////////////////////////////////
	
	inline double GetGameTimeElapsed() const
	{
		return m_TotalElapsedT;
	}
	inline float GetDeltaT() const
	{
		return (float)m_Dt;
	}
	inline sf::RenderWindow& GetRenderWin() const
	{
		return *m_puRenderWin;
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
	void Init();
	void Run();
	
	//////////////////////////////////////////////////
	
private:

	void InitRenderWin();
	void Update();
	void Render();
	inline void	Terminate()
	{
		if ( m_puRenderWin != nullptr )
		{
			if ( m_puRenderWin->isOpen() == true )
				m_puRenderWin->close();
		}
	}

	//////////////////////////////////////////////////

	str m_AppName;
	double m_TotalElapsedT, m_Dt;
	intU m_WinWidth, m_WinHeight, m_FramesPerSec, m_Aa;
	bool m_VSync;
	pU<sf::RenderWindow> m_puRenderWin;
	pU<sf::ContextSettings> m_puCSettings;
	pU<sf::Event> m_puEvent;
	pU<sf::Clock> m_puClock;

#ifdef DEBUG
	grSandbox* m_pSandbox;
#endif // DEBUG
};

#endif		//_GRCORE_H_

