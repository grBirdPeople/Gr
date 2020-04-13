#include	"grCore.h"

#include	"grDebugManager.h"
#include	"grDefine.h"
#include	"grEntityManager.h"
#include	"grInputManager.h"
#include	"grMapManager.h"
#include	"grNavMeshManager.h"
#include	"grParticleManager.h"
#include	"grParticleManagerPB.h"
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
	InitRenderWin();

	//////////////////////////////////////////////////

	grInputManager::Initialize();
	grEntityManager::Initialize();
	grMapManager::Initialize();
	grNavMeshManager::Initialize();
	//grParticleManager::Initialize();
	grParticleManagerPB::Initialize();

#ifdef DEBUG
	grDebugManager::Initialize();
#endif // DEBUG
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

	grParticleManagerPB::DeInitialize();
	//grParticleManager::DeInitialize();
	grNavMeshManager::DeInitialize();
	grMapManager::DeInitialize();
	grEntityManager::DeInitialize();
	grInputManager::DeInitialize();

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
	//grParticleManager::Instance().Update( GetDeltaT() );
	grParticleManagerPB::Instance().Update( GetDeltaT() );

#ifdef DEBUG
	grNavMeshManager::Instance().Debug();
#endif // DEBUG
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


// Init
//////////////////////////////////////////////////
void
grCore::Init( void )
{
#ifdef	DEBUG
	m_pSandbox = new grSandbox();
#endif	//DEBUG_MODE
}


// Run
//////////////////////////////////////////////////
void
grCore::Run( void )
{
	double timeLast	= m_pEngineClock->getElapsedTime().asSeconds();
	grInputManager& rInputMan = grInputManager::Instance();

	while ( m_pRenderWin->isOpen() )
	{
		while ( m_pRenderWin->pollEvent( *m_pSfEvent ) )
		{
			switch( m_pSfEvent->type )
			{
				case eEvent::Closed: Terminate(); break;
				// Input
				case eEvent::KeyPressed: rInputMan.SetKeyDown( m_pSfEvent->key.code ); break;
				case eEvent::KeyReleased: rInputMan.SetKeyUp( m_pSfEvent->key.code ); break;
				case eEvent::MouseMoved: rInputMan.SetMouseMoved( grV2f( ( float )m_pSfEvent->mouseMove.x, ( float )m_pSfEvent->mouseMove.y ) ); break;																	break;
				case eEvent::MouseButtonPressed: rInputMan.SetMouseDown( m_pSfEvent->mouseButton.button ); break;
				case eEvent::MouseButtonReleased: rInputMan.SetMouseUp( m_pSfEvent->mouseButton.button ); break;
				case eEvent::MouseWheelScrolled: rInputMan.SetMouseScroll( ( uInt )m_pSfEvent->mouseWheelScroll.delta ); break;
			};
		}
		
		m_TotalTimeElapsed = m_pEngineClock->getElapsedTime().asSeconds();
		m_Dt = m_TotalTimeElapsed - timeLast;
		timeLast = m_TotalTimeElapsed;
		
		Update();
		Render();
		rInputMan.Update();
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

