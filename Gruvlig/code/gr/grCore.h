#ifndef		_GRCORE_H_
#define		_GRCORE_H_

#define		AA_DEFAULT	8	// 8 is max for SFML

#include	<SFML/Graphics/RenderWindow.hpp>
#include	<SFML/Window/ContextSettings.hpp>
#include	<SFML/Window/Event.hpp>
#include	<SFML/System/Clock.hpp>

#include	"grCommon.h"
#include	"grSingleton.h"

typedef		sf::ContextSettings	cSettings;
typedef		sf::Clock			gClock;
typedef		sf::Event			eEvent;
typedef		sf::RenderWindow	rWin;

class		grSandbox;


// grCore
//////////////////////////////////////////////////
class grCore : public grSingleton<grCore>
{
public:

	grCore( const intU winWidth = 640, const intU winHeight = 360, const intU frameRate = 60, const str& rAppName = "grFramework" );
	//grCore( const intU winWidth = 1920, const intU winHeight = 1080, const intU frameRate = 60, const str& rAppName = "grFramework" );
	~grCore( void );

	//////////////////////////////////////////////////
	
	inline double GetGameTimeElapsed( void ) const
	{
		return m_TotalTimeElapsed;
	}
	inline float GetDeltaT( void ) const
	{
		return (float)m_Dt;
	}
	inline rWin& GetRenderWin( void ) const
	{
		return *m_pRenderWin;
	}
										  
	inline void SetAA ( const intU aa )
	{
		m_Aa = aa;
		m_pCSettings->antialiasingLevel = ( unsigned int )m_Aa;
	}		 							  
	inline void SetFramerateLimit( const intU frameRateLimit )
	{
		m_FramesPerSec = frameRateLimit;
		m_pRenderWin->setFramerateLimit( ( unsigned int )m_FramesPerSec );
	}
	inline void SetVSync( const bool vSync )
	{
		m_VSync = vSync;
		m_pRenderWin->setVerticalSyncEnabled( m_VSync );
	}
					
	void Init( void );
	void Run( void );
	
	//////////////////////////////////////////////////
	
private:

	void InitRenderWin( void );
	void Update	( void );
	void Render( void );
	inline void	Terminate( void )
	{
		if ( m_pRenderWin != nullptr )
		{
			if ( m_pRenderWin->isOpen() == true )
			{
				m_pRenderWin->close();
			}
		}
	}

	//////////////////////////////////////////////////

	str			m_AppName;
	
	double		m_TotalTimeElapsed,
				m_Dt;
	
	intU		m_WinWidth,
				m_WinHeight,
				m_FramesPerSec,
				m_Aa;
				
	bool		m_VSync;
	
	rWin*		m_pRenderWin;
	cSettings*	m_pCSettings;
	eEvent*		m_pSfEvent;
	gClock*		m_pEngineClock;

#ifdef DEBUG
	grSandbox* m_pSandbox;
#endif // DEBUG

};

#endif		//_GRCORE_H_

