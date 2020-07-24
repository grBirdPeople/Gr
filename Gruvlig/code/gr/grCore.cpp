#include	"grCore.h"

#include	"grDebugManager.h"
#include	"grEntityManager.h"
#include	"grInputManager.h"
#include	"grMapManager.h"
#include	"grNavMeshManager.h"
//#include	"grParticleManagerPB.h"
//#include	"grCParticleManager.h"
#include	"grSandbox.h"
#include	"grStruct.h"


// cTor
//////////////////////////////////////////////////
grCore::grCore( const intU winWidth, const intU winHeight, const intU frameRate, const str& rAppName )
	: m_AppName			( rAppName )
	, m_TotalElapsedT	( 0.0 )
	, m_Dt				( 0.0 )
	, m_WinWidth		( winWidth )
	, m_WinHeight		( winHeight )
	, m_FramesPerSec	( frameRate )
	, m_Aa				( AA_DEFAULT )
	, m_VSync			( true )
	, m_puRenderWin		( std::make_unique<sf::RenderWindow>() )
	, m_puCSettings		( std::make_unique<sf::ContextSettings>() )
	, m_puEvent			( std::make_unique<sf::Event>() )
	, m_puClock			( std::make_unique<sf::Clock>() )
{
	InitRenderWin();

	//////////////////////////////////////////////////

	grInputManager::Initialize();
	grEntityManager::Initialize();
	grMapManager::Initialize();
	grNavMeshManager::Initialize();
	//grCParticleManagerPB::Initialize();
	//grCParticleManager::Initialize();

#ifdef DEBUG
	grDebugManager::Initialize();
	grDebugManager::Instance().Enable( false );
#endif // DEBUG
}


// dTor
//////////////////////////////////////////////////
grCore::~grCore()
{
#ifdef DEBUG
	if ( m_pSandbox != nullptr )
		DELANDNULL( m_pSandbox );

	grDebugManager::DeInitialize();
#endif // DEBUG

	//grCParticleManager::DeInitialize();
	//grCParticleManagerPB::DeInitialize();
	//grParticleManager::DeInitialize();
	grNavMeshManager::DeInitialize();
	grMapManager::DeInitialize();
	grEntityManager::DeInitialize();
	grInputManager::DeInitialize();

	//////////////////////////////////////////////////

	if ( m_puRenderWin )
	{
		if ( m_puRenderWin->isOpen() == true )
			m_puRenderWin->close();
	}
}


// Update
//////////////////////////////////////////////////
void
grCore::Update()
{
#ifdef DEBUG
	m_pSandbox->Update( GetDeltaT() );
	grNavMeshManager::Instance().Debug();
#endif // DEBUG

	// Update entities
	grEntityManager::Instance().Update( GetDeltaT() );
	//grParticleManager::Instance().Update( GetDeltaT() );
	//grCParticleManagerPB::Instance().Update( GetDeltaT() );
	//grCParticleManager::Instance().Update( GetDeltaT() );
}


// Render
//////////////////////////////////////////////////
void
grCore::Render()	
{
	m_puRenderWin->clear();
	
	//////////////////////////////////////////////////
	
#ifdef	DEBUG
	m_pSandbox->Render( *m_puRenderWin );
	grDebugManager::Instance().Render( *m_puRenderWin );
#endif	//DEBUG_MODE

	//////////////////////////////////////////////////
	
	m_puRenderWin->display();
}


// Init
//////////////////////////////////////////////////
void
grCore::Init()
{
#ifdef	DEBUG
	m_pSandbox = new grSandbox();
#endif	//DEBUG_MODE
}


// Run
//////////////////////////////////////////////////
void
grCore::Run()
{
	double timeLast	= m_puClock->getElapsedTime().asSeconds();
	grInputManager& rInputMan = grInputManager::Instance();

	while ( m_puRenderWin->isOpen() )
	{
		while ( m_puRenderWin->pollEvent( *m_puEvent ) )
		{
			switch( m_puEvent->type )
			{
				//case sf::Event::Closed: Terminate(); break;
				// Input
				case sf::Event::KeyPressed: rInputMan.SetKeyDown( m_puEvent->key.code ); break;
				case sf::Event::KeyReleased: rInputMan.SetKeyUp( m_puEvent->key.code ); break;
				case sf::Event::MouseMoved: rInputMan.SetMouseMoved( grV2f( ( float )m_puEvent->mouseMove.x, ( float )m_puEvent->mouseMove.y ) ); break;																	break;
				case sf::Event::MouseButtonPressed: rInputMan.SetMouseDown( m_puEvent->mouseButton.button ); break;
				case sf::Event::MouseButtonReleased: rInputMan.SetMouseUp( m_puEvent->mouseButton.button ); break;
				case sf::Event::MouseWheelScrolled: rInputMan.SetMouseScroll( ( intU )m_puEvent->mouseWheelScroll.delta ); break;
			};
		}
		
		m_TotalElapsedT = m_puClock->getElapsedTime().asSeconds();
		m_Dt = m_TotalElapsedT - timeLast;
		timeLast = m_TotalElapsedT;
		
		Update();
		Render();
		rInputMan.Update();
	}
}


// InitRWin
//////////////////////////////////////////////////
void
grCore::InitRenderWin()
{
	m_puCSettings->antialiasingLevel = ( unsigned int )m_Aa;
	m_puRenderWin->create( sf::VideoMode( ( unsigned int )m_WinWidth, ( unsigned int )m_WinHeight ), m_AppName,sf::Style::None, *m_puCSettings );
	//m_puRenderWin->setFramerateLimit( m_FramesPerSec );
	//m_puRenderWin->setVerticalSyncEnabled( true );
}

