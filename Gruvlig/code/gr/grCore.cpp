#include	"grCore.h"

#include	"grDebugManager.h"
#include	"grDefine.h"
#include	"grInput.h"
#include	"grEntityManager.h"
#include	"grMapManager.h"
#include	"grNavMeshManager.h"
#include	"grSandbox.h"


// cTor
//////////////////////////////////////////////////
grCore::grCore( const uInt winWidth, const uInt winHeight, const uInt frameRate, const str& rAppName )
	: m_AppName				( rAppName )
	, m_TotalTimeElapsed	( 0.0 )
	, m_Dt					( 0.0 )
	, m_WinWidth			( winWidth )
	, m_WinHeight			( winHeight )
	, m_FramesPerSec		( frameRate )
	, m_Aa					( AA_DEFAULT )
	, m_VSync				( true )
	, m_pRenderWin			( new sf::RenderWindow )
	, m_pCSettings			( new sf::ContextSettings )
	, m_pSfEvent			( new sf::Event )
	, m_pEngineClock		( new sf::Clock )
{
	grEntityManager::Initialize();
	grMapManager::Initialize();
	grNavMeshManager::Initialize();
	grInput::Initialize();

#ifdef DEBUG
	grDebugManager::Initialize();

	m_pSandbox = new grSandbox();
#endif // DEBUG

	//////////////////////////////////////////////////

	InitRenderWin();

}


// dTor
//////////////////////////////////////////////////
grCore::~grCore( void )
{
#ifdef DEBUG
	if ( m_pSandbox != nullptr )
	{
		DELANDNULL( m_pSandbox );
	}

	grDebugManager::DeInitialize();
#endif // DEBUG

	grInput::DeInitialize();
	grNavMeshManager::DeInitialize();
	grMapManager::DeInitialize();
	grEntityManager::DeInitialize();

	//////////////////////////////////////////////////

	if ( m_pRenderWin )
	{
		if ( m_pRenderWin->isOpen() == true )
		{
			m_pRenderWin->close();
		}

		DELANDNULL( m_pRenderWin );
	}

	if ( m_pSfEvent )		{ DELANDNULL( m_pSfEvent ); }
	if ( m_pEngineClock )	{ DELANDNULL( m_pEngineClock ); }
	if ( m_pCSettings )		{ DELANDNULL( m_pCSettings ); }
}


// Update
//////////////////////////////////////////////////
void
grCore::Update( void )
{
#ifdef DEBUG
	m_pSandbox->Update( GetDeltaT() );
#endif // DEBUG

	// Update entities
	grEntityManager::Instance().Update( GetDeltaT() );
}


// Render
//////////////////////////////////////////////////
void
grCore::Render( void )	
{
	m_pRenderWin->clear();
	
	//////////////////////////////////////////////////
	
#ifdef	DEBUG
	m_pSandbox->Render( *m_pRenderWin );
	grDebugManager::Instance().DebugRender( *m_pRenderWin );
#endif	//DEBUG_MODE

	//////////////////////////////////////////////////
	
	m_pRenderWin->display();
}


// Run
//////////////////////////////////////////////////
void
grCore::Run( void )
{
	grInput& rInput	= grInput::Instance();
	double timeLast	= m_pEngineClock->getElapsedTime().asSeconds();
	
	while ( m_pRenderWin->isOpen() )
	{
		while ( m_pRenderWin->pollEvent( *m_pSfEvent ) )
		{
			switch( m_pSfEvent->type )
			{
				case eEvent::Closed:				Terminate();											break;
				case eEvent::KeyPressed:			rInput.SetKeyDown( m_pSfEvent->key.code );				break;
				case eEvent::KeyReleased:			rInput.SetKeyUp( m_pSfEvent->key.code );				break;
				case eEvent::MouseButtonPressed:	rInput.SetMouseDown( m_pSfEvent->mouseButton.button );	break;
				case eEvent::MouseButtonReleased:	rInput.SetMouseUp( m_pSfEvent->mouseButton.button );	break;
			};
		}
		
		m_TotalTimeElapsed = m_pEngineClock->getElapsedTime().asSeconds();
		m_Dt = m_TotalTimeElapsed - timeLast;
		timeLast = m_TotalTimeElapsed;
		
		Update();
		Render();
	}
}


// InitRWin
//////////////////////////////////////////////////
void
grCore::InitRenderWin( void )
{
	m_pCSettings->antialiasingLevel = m_Aa;
	m_pRenderWin->create( sf::VideoMode( m_WinWidth, m_WinHeight ), m_AppName,sf::Style::None, *m_pCSettings );
	//m_pRenderWin->setFramerateLimit(m_FramesPerSec);
	m_pRenderWin->setVerticalSyncEnabled( true );
}

