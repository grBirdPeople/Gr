#include	"grCore.h"

#include	"grDebugManager.h"
#include	"grDefine.h"
#include	"grEntityManager.h"
#include	"grInput.h"
#include	"grMapManager.h"
#include	"grNavMeshManager.h"
#include	"grParticleManager.h"
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
	grInput::Initialize();
	m_pInputMan = grInput::InstancePtr();
	grEntityManager::Initialize();
	grMapManager::Initialize();
	grNavMeshManager::Initialize();
	grParticleManager::Initialize();

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

	grParticleManager::DeInitialize();
	grNavMeshManager::DeInitialize();
	grMapManager::DeInitialize();
	grEntityManager::DeInitialize();
	m_pInputMan = nullptr;
	grInput::DeInitialize();

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
	double timeLast	= m_pEngineClock->getElapsedTime().asSeconds();
	
	while ( m_pRenderWin->isOpen() )
	{
		while ( m_pRenderWin->pollEvent( *m_pSfEvent ) )
		{
			switch( m_pSfEvent->type )
			{
				case eEvent::Closed:				Terminate();																break;
				// Input
				case eEvent::KeyPressed:			m_pInputMan->SetKeyDown( m_pSfEvent->key.code );							break;
				case eEvent::KeyReleased:			m_pInputMan->SetKeyUp( m_pSfEvent->key.code );								break;
				case eEvent::MouseMoved:			m_pInputMan->SetMouseMoved();												break;
				case eEvent::MouseButtonPressed:	m_pInputMan->SetMouseDown( m_pSfEvent->mouseButton.button );				break;
				case eEvent::MouseButtonReleased:	m_pInputMan->SetMouseUp( m_pSfEvent->mouseButton.button );					break;
				case eEvent::MouseWheelScrolled:	m_pInputMan->SetMouseScroll( ( uInt )m_pSfEvent->mouseWheelScroll.delta );	break;
			};
		}
		
		m_TotalTimeElapsed = m_pEngineClock->getElapsedTime().asSeconds();
		m_Dt = m_TotalTimeElapsed - timeLast;
		timeLast = m_TotalTimeElapsed;
		
		Update();
		Render();
		m_pInputMan->Update();
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

