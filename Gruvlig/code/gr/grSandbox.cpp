#include "grSandbox.h"

#include	<SFML/Window/Mouse.hpp>

#include	"grCore.h"
#include	"grBox2D.h"
#include	"grDebugManager.h"
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


// cTor
//////////////////////////////////////////////////
grSandbox::grSandbox( void )
{
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
	grIEntity* pEnemy_4 = entityMan.CreateEntity( grEnums::EntityType::ENEMY, "Enemy_4", grV2f( 320.0f, 130.0f ) );

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
}


// Update
//////////////////////////////////////////////////
void
grSandbox::Update( const float deltaT )
{
	grCore& core = grCore::Instance();
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

		( hej == false ) ? m_pPlayer->ReleaseChildById( m_pEnemy->GetId() ) : m_pPlayer->AddChild( m_pEnemy );
	}


	// A*
	if ( m_pMap != nullptr )
	{
		if ( input.GetMouse( sf::Mouse::Button::Left ) )
		{
			grV2f mousePos = grV2f( ( float )sf::Mouse::getPosition( core.GetRenderWin() ).x, ( float )sf::Mouse::getPosition( core.GetRenderWin() ).y );
			m_pActor->SetEnd( m_pMap, mousePos );
			m_pActor->FindPath( m_pMap );
		}

		if ( input.GetMouse( sf::Mouse::Button::Right ) )
		{
			grV2f mousePos = grV2f( ( float )sf::Mouse::getPosition( core.GetRenderWin() ).x, ( float )sf::Mouse::getPosition( core.GetRenderWin() ).y );
			m_pActor->SetStart( m_pMap, mousePos );
			m_pActor->FindPath( m_pMap );
		}
	}
}


// Render
//////////////////////////////////////////////////
void
grSandbox::Render( sf::RenderWindow& rRenderWin )
{
#ifdef	DEBUG
	grNavMeshManager::Instance( ).GetNavMesh( m_pMap->GetMapData()->Name )->DebugRender();
#endif	//DEBUG_MODE
}