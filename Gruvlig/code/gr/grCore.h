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

class		grInput;
class		grSandbox;


// grCore
//////////////////////////////////////////////////
class grCore : public grSingleton<grCore>
{
public:

					grCore				( const uInt winWidth = 640, const uInt winHeight = 360, const uInt frameRate = 60, const str& rAppName = "grFramework" );
					~grCore				( void );

	//////////////////////////////////////////////////
	
	inline	double	GetGameTimeElapsed	( void )	const				{ return m_TotalTimeElapsed; }
	inline	float	GetDeltaT			( void )	const				{ return (float)m_Dt; }
	inline	rWin&	GetRenderWin		( void )	const				{ return *m_pRenderWin; }
										  
	inline	void	SetAA				( const uInt aa )				{ m_Aa = aa;	m_pCSettings->antialiasingLevel = m_Aa; }
										  
	inline	void	SetFramerateLimit	( const uInt frameRateLimit )	{ m_FramesPerSec = frameRateLimit;	m_pRenderWin->setFramerateLimit( m_FramesPerSec ); }
	inline	void	SetVSync			( const bool vSync )			{ m_VSync = vSync;	m_pRenderWin->setVerticalSyncEnabled( m_VSync ); }
										  
			void	Run					( void );
	
	//////////////////////////////////////////////////
	
private:

			void	InitRenderWin		( void );
			void	Update				( void );
			void	Render				( void );
	inline	void	Terminate			( void )						{ if ( m_pRenderWin != nullptr ) { if ( m_pRenderWin->isOpen() == true ) { m_pRenderWin->close(); } } }

	//////////////////////////////////////////////////

	str			m_AppName;
	
	double		m_TotalTimeElapsed,
				m_Dt;
	
	uInt		m_WinWidth,
				m_WinHeight,
				m_FramesPerSec,
				m_Aa;
				
	bool		m_VSync;
	
	rWin*		m_pRenderWin;
	cSettings*	m_pCSettings;
	eEvent*		m_pSfEvent;
	gClock*		m_pEngineClock;
	grInput*	m_pInputMan;

#ifdef DEBUG
	grSandbox* m_pSandbox;
#endif // DEBUG

};

#endif		//_GRCORE_H_

