#include	"grCore.h"

#include	"grDebugManager.h"
#include	"grEntityManager.h"
#include	"grInputManager.h"
#include	"grMapManager.h"
#include	"grNavMeshManager.h"
#include	"grSandbox.h"
#include	"grStruct.h"


// cTor
//////////////////////////////////////////////////
grCore::grCore()
	: m_puRenderWin( std::make_unique<sf::RenderWindow>() )
	, m_puCSettings( std::make_unique<sf::ContextSettings>() )
	, m_puEvent( std::make_unique<sf::Event>() )
	, m_puVidMode( std::make_unique<sf::VideoMode>() )
	, m_puClock( std::make_unique<sf::Clock>() )
	, m_AppName( "grFramework" )
	, m_WindowedSize( { 0, 0 } )
	, m_TotalElapsedT( 0.0f )
	, m_Dt( 0.0f )
	, m_FramesPerSec( 0 )
	, m_Aa( AA_MAX )
	, m_VSync( false )
#ifdef DEBUG
	, m_puSandbox( nullptr )
#endif // DEBUG
{}


// dTor
//////////////////////////////////////////////////
grCore::~grCore()
{
	DeInitManager();
}


// Input
//////////////////////////////////////////////////
inline void
grCore::Input( grInputManager& rInputMan )
{
	rInputMan.Update(); // Resets some vars for mouse

	while ( m_puRenderWin->pollEvent( *m_puEvent ) )
	{
		switch ( m_puEvent->type )
		{
			// Input
			case sf::Event::KeyPressed: rInputMan.SetKeyDown( m_puEvent->key.code ); break;
			case sf::Event::KeyReleased: rInputMan.SetKeyUp( m_puEvent->key.code ); break;
			case sf::Event::MouseMoved: rInputMan.SetMouseMoved( grV2f( ( float )m_puEvent->mouseMove.x, ( float )m_puEvent->mouseMove.y ) ); break;
			case sf::Event::MouseButtonPressed: rInputMan.SetMouseDown( m_puEvent->mouseButton.button ); break;
			case sf::Event::MouseButtonReleased: rInputMan.SetMouseUp( m_puEvent->mouseButton.button ); break;
			case sf::Event::MouseWheelScrolled: rInputMan.SetMouseScroll( ( intU )m_puEvent->mouseWheelScroll.delta ); break;
		};
	}

#ifdef DEBUG
	if ( rInputMan.GetKeyDown( sf::Keyboard::F1 ) ) // Fullscreen toggle
	{
		if ( m_puVidMode->width == m_WindowedSize.x && m_puVidMode->height == m_WindowedSize.y )
			SetWindowSize( { ( intU )m_puVidMode->getDesktopMode().width, ( intU )m_puVidMode->getDesktopMode().height } );
		else
			SetWindowSize( m_WindowedSize );
	}

	if ( rInputMan.GetKeyDown( sf::Keyboard::F2 ) ) // Vsync toggle
		SetVSync( !m_VSync );

	if ( rInputMan.GetKeyDown( sf::Keyboard::F3 ) ) // Debugrender toggle
	{
		grDebugManager& d = grDebugManager::Instance();
		d.Enable( !d.IsEnable() );
	}

	if ( rInputMan.GetKeyDown( sf::Keyboard::Escape ) ) // Exit
		Terminate();
#endif // DEBUG
}


// Update
//////////////////////////////////////////////////
inline void
grCore::Update()
{
#ifdef DEBUG
	m_puSandbox->Update( GetDeltaT() );
	grNavMeshManager::Instance().Debug();
#endif // DEBUG

	grEntityManager::Instance().Update( GetDeltaT() );
}


// Render
//////////////////////////////////////////////////
inline void
grCore::Render()	
{
	m_puRenderWin->clear();
	
#ifdef	DEBUG
	m_puSandbox->Render( *m_puRenderWin );
	grDebugManager::Instance().Render( *m_puRenderWin );
#endif	//DEBUG_MODE
	
	m_puRenderWin->display();
}


// Terminate
//////////////////////////////////////////////////
inline void
grCore::Terminate()
{
	if ( m_puRenderWin != nullptr )
	{
		if ( m_puRenderWin->isOpen() == true )
			m_puRenderWin->close();
	}
}


// Init
//////////////////////////////////////////////////
void
grCore::Init( const str& rAppName, const intU winWidth, const intU winHeight, const intU frameRate, const bool vsync )
{
	m_AppName = rAppName;
	m_WindowedSize = { winWidth, winHeight };
	m_FramesPerSec = frameRate;
	m_VSync = vsync;

	InitRenderWin();
	InitManager();

#ifdef	DEBUG
	m_puSandbox = std::make_unique<grSandbox>();
#endif	//DEBUG_MODE
}


// Run
//////////////////////////////////////////////////
void
grCore::Run()
{
	grInputManager& rInputMan = grInputManager::Instance();
	float timeLast	= m_puClock->getElapsedTime().asSeconds();

	while ( m_puRenderWin->isOpen() )
	{
		m_TotalElapsedT = m_puClock->getElapsedTime().asSeconds();
		m_Dt = m_TotalElapsedT - timeLast;
		timeLast = m_TotalElapsedT;
		
		Input( rInputMan );
		Update();
		Render();
	}
}


// InitRWin
//////////////////////////////////////////////////
inline void
grCore::InitRenderWin()
{
	m_puVidMode->width = ( unsigned int )m_WindowedSize.x;
	m_puVidMode->height = ( unsigned int )m_WindowedSize.y;
	m_puVidMode->bitsPerPixel = 32;

	m_puCSettings->antialiasingLevel = ( unsigned int )m_Aa;

	m_puRenderWin->create( *m_puVidMode, m_AppName, sf::Style::None, *m_puCSettings );
	m_puRenderWin->setFramerateLimit( ( unsigned int )m_FramesPerSec );
	m_puRenderWin->setVerticalSyncEnabled( m_VSync );
}


// InitManager
//////////////////////////////////////////////////
inline void
grCore::InitManager()
{
	grInputManager::Initialize();
	grEntityManager::Initialize();
	grMapManager::Initialize();
	grNavMeshManager::Initialize();

#ifdef DEBUG
	grDebugManager::Initialize();
#endif // DEBUG
}


inline void
grCore::DeInitManager()
{
#ifdef DEBUG
	grDebugManager::DeInitialize();
#endif // DEBUG

	grNavMeshManager::DeInitialize();
	grMapManager::DeInitialize();
	grEntityManager::DeInitialize();
	grInputManager::DeInitialize();

	if ( m_puRenderWin != nullptr )
	{
		if ( m_puRenderWin->isOpen() == true )
			m_puRenderWin->close();
	}
}
