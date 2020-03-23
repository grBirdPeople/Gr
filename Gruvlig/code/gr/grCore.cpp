#include	"grCore.h"

#include	"grDebugManager.h"
#include	"grDefine.h"
#include	"grInput.h"

// TEST
#include	<SFML/Window/Mouse.hpp>
#include	"grBox2D.h"

#include	"grInput.h"
#include	"grActor.h"
#include	"grMap.h"
#include	"grNavMesh.h"
#include	"grNavMeshManager.h"
#include	"grNavNode.h"
#include	"grIEntity.h"
#include	"grEntityPlayer.h"
#include	"grEntityManager.h"
#include	"grMapManager.h"
#include	"grHashMap.h"
// TEST


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
#endif // DEBUG

	//////////////////////////////////////////////////

	InitRenderWin();

	//////////////////////////////////////////////////

	// TEST

	// CReate maps and navmeshes
	grMapManager::Instance().GetMap( "m_01" )->CreateNavMesh();
	grMapManager::Instance().GetMap( "m_02" )->CreateNavMesh();
	m_pMap = grMapManager::Instance().GetMap( "m_02" );

	// Create an actor and add it to a map
	m_pActor = new grActor( m_pMap );

	// Entities
	grEntityManager& entityMan = grEntityManager::Instance();

	m_pPlayer = entityMan.CreateEntity( grEnums::EntityType::PLAYER, "Player", grV2f( 300.0f, 150.0f ) );

	grIEntity* pEnemy_1 = entityMan.CreateEntity( grEnums::EntityType::ENEMY, "Enemy_1", grV2f( 290.0f, 140.0f ) );
	grIEntity* pEnemy_2 = entityMan.CreateEntity( grEnums::EntityType::ENEMY, "Enemy_2", grV2f( 280.0f, 130.0f ) );
	m_pEnemy = entityMan.CreateEntity( grEnums::EntityType::ENEMY, "Enemy_3", grV2f( 310.0f, 140.0f ) );
	grIEntity* pEnemy_4 = entityMan.CreateEntity( grEnums::EntityType::ENEMY, "Enemy_4", grV2f(320.0f, 130.0f ) );

	m_pPlayer->AddChild( pEnemy_1 );
	pEnemy_1->AddChild( pEnemy_2 );
	m_pPlayer->AddChild( m_pEnemy );
	m_pEnemy->AddChild( pEnemy_4 );
	//m_pPlayer->ReleaseChildById( pEnemy_1->GetId() );
	//pPlayer->ReleaseChildByIdx( 0 );

	//m_pPlayer->SetEnable( false );
	//entityMan.DestroyEntity( pEnemy_1 );



	//// HashMap
	//grHashMap<grIEntity*> map( 5 );
	//map.Put( 3, m_pPlayer );
	//map.Put( 3, m_pEnemy );
	//grIEntity* pTmp = map.Get( 7 );
	//pTmp = map.Get( 3 );
	//std::vector<uInt> used = map.UsedKeys();
	//map.Del( 7 );
	//map.Del( 3 );
	//map.Put( 3, m_pPlayer );



	//// Box2D test
	//b2World* pWorld = new b2World( b2Vec2( 0.0f, -9.8f ) );

	//b2Body* pBody = nullptr;
	//b2BodyDef bodyDef;
	//bodyDef.type = b2_dynamicBody;
	//bodyDef.position.Set( (float)m_WinWidth * 0.5f, (float)m_WinHeight * 0.5f );
	//pBody = pWorld->CreateBody(&bodyDef);

	//b2PolygonShape sqrShape;
	//sqrShape.SetAsBox(50.0f * 0.5f, 50.0f * 0.5f);

	//b2FixtureDef fixDef;
	//fixDef.shape = &sqrShape;
	//fixDef.density = 1.0f;
	//fixDef.friction = 0.5f;
	//b2Fixture* pFix = pBody->CreateFixture( &fixDef );

	// TEST
}


// dTor
//////////////////////////////////////////////////
grCore::~grCore( void )
{
#ifdef DEBUG
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


// HandleInput
//////////////////////////////////////////////////
void
grCore::HandleInput( void )
{
	// TEST
	grInput& input = grInput::Instance();

	// SceneGraph stuff
	if ( input.GetMouseDown( sf::Mouse::Left ) )
	{
		//m_pPlayer->ReleaseChildByIdx( 0 );		
		//grEntityManager::Instance().DestroyEntity( m_pPlayer );
		//grEntityManager::Instance().DestroyEntity( m_pEnemy );

		hej = !hej;
		//m_pEnemy->SetEnable( hej );
		//m_pPlayer->SetEnable( hej );

		(hej == false) ? m_pPlayer->ReleaseChildById(m_pEnemy->GetId()) : m_pPlayer->AddChild(m_pEnemy);
	}


	// A*
	if ( m_pMap != nullptr )
	{
		if ( input.GetMouse( sf::Mouse::Button::Left ) )
		{
			grV2f mousePos = grV2f( (float)sf::Mouse::getPosition( *m_pRenderWin ).x, (float)sf::Mouse::getPosition( *m_pRenderWin ).y );
			m_pActor->SetEnd( m_pMap, mousePos );
			m_pActor->FindPath( m_pMap );
		}

		if ( input.GetMouse( sf::Mouse::Button::Right ) )
		{
			grV2f mousePos = grV2f( (float )sf::Mouse::getPosition( *m_pRenderWin ).x, (float)sf::Mouse::getPosition( *m_pRenderWin ).y );
			m_pActor->SetStart( m_pMap, mousePos );
			m_pActor->FindPath( m_pMap );
		}
	}

	// TEST
}


// Update
//////////////////////////////////////////////////
void
grCore::Update( void )
{
	HandleInput();

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
	grNavMeshManager::Instance().GetNavMesh( m_pMap->GetMapData()->Name )->DebugRender();
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
		
		HandleInput();
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

