#include "grSandbox.h"

#include	<SFML/Window/Mouse.hpp>
#include	<SFML/Graphics/Sprite.hpp>

#include	"grCore.h"
#include	"grBox2D.h"
#include	"grDebugManager.h"
#include	"grInputManager.h"
#include	"grActor.h"
#include	"grMap.h"
#include	"grNavMesh.h"
#include	"grNavMeshManager.h"
#include	"grNavNode.h"
#include	"grIEntity.h"
#include	"grEntityPlayer.h"
#include	"grEntityManager.h"
#include	"grMapManager.h"
#include	"grCParticleManager.h"
#include	"grMath.h"


// cTor
//////////////////////////////////////////////////
grSandbox::grSandbox( void )
	: m_rInputMan		( grInputManager::Instance() )
	, m_rPartMan		( grCParticleManager::Instance() )
	, m_PartSysIdOne	( -1 )
	, m_PartSysIdTwo	( -1 )
{
	// Create maps and navmeshes
	grMapManager::Instance().GetMap( "map_00" )->CreateNavMesh();	// TODO: Don't like this. Other way around ( navmeshmanager create navmesh ( map ))
	grMapManager::Instance().GetMap( "map_01" )->CreateNavMesh();

	m_pMap = grMapManager::Instance().GetMap( "map_00" );
	grNavMeshManager::Instance().SetNavMeshToDebug( -1 );	// TODO: Don't know if I want to keep the map stuff,
															// but maybe better if this is called auto in grMap and also updated from there instead of in core

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

	// Particles
	//grParticleSystem& pPartSys = *m_rParticleMan.CreateParticleSystem();
	//pPartSys.Init( grV2f( 350.0f, 150.0f ), grV2f( 1.0f, -1.0f ), 50.0f, 2.0f, 4 );

	// Particles Physics Based
	//grParticleAttributePB partAtt;
	//partAtt.SetSpeed( 10000.0f, 12500.0f, -100.0f );
	//partAtt.SetDirection( 337.5f, 22.5f );
	//partAtt.SetGravity( 400.0f, 270.0f );
	//partAtt.SetLifetime( 1.0f, 1.25f );
	//m_rParticleSetup.SetParticleAttribute( partAtt );

	// Particles another one
	//m_pPartEmitter_1 = m_rPartMan.CreateEmitter();
	//m_pPartEmitter_2 = m_rPartMan.CreateEmitter();

	//grCParticleAttributePB attOne;
	//grCParticleAttributePB attTwo;

	//attOne.Color = grSColor( 0, 175, 250, 75 );
	//attOne.Position = grV2f( grCore::Instance().GetRenderWin().getSize().x * 0.5f, grCore::Instance().GetRenderWin().getSize().y * 0.5f );
	//attOne.Velocity = grV2f( 0.0f, -1.0f ) * 100.0f;
	//attOne.Lifetime = 2.5f;

	//attTwo.Color = grSColor( 250, 175, 0, 150 );
	//attTwo.Position = grV2f( grCore::Instance().GetRenderWin().getSize().x * 0.25f, grCore::Instance().GetRenderWin().getSize().y * 0.5f );
	//attTwo.Velocity = grV2f( -1.0f, 0.0f ) * 100.0f;
	//attTwo.Lifetime = 0.5f;

	//m_pPartEmitter_1->SetParticleAttribute( attOne );
	//m_pPartEmitter_2->SetParticleAttribute( attTwo );




	//grStruct::grLinearActivity<grSParticle> arr( 5 );
	//for ( sizeT i = 0; i < 5; ++i )
	//{
	//	arr.Push( grSParticle() );
	//	arr[ i ].Lifetime = i;
	//}

	//for ( sizeT i = 0; i < 5; ++i )
	//{
	//	arr.Activate();
	//}

	//for ( sizeT i = 0; i < 5; ++i )
	//{
	//	arr.Deactivate( i );
	//	int life = arr[ i ].Lifetime;
	//	int j = 7;
	//}

	grCParticleEmitter& emitr_1 = grCParticleManager::Instance().Create();
	int j = 7;
}




// Update
//////////////////////////////////////////////////
void
grSandbox::Update( const float deltaT )
{
	// Particle things




	//if ( m_rInputMan.GetMouseMoved() == true )
	//{
	//	grCParticleAttributePB att = m_pPartEmitter_1->GetParticleAttribute();
	//	att.Position = grMath::Lerp( att.Position, m_rInputMan.GetMousePos(), 0.75f );
	//	att.Velocity = grV2f( att.Position - m_rInputMan.GetMousePos() ).Normalized() * 50.0f;
	//	att.Lifetime = 3.0f;
	//	m_pPartEmitter_1->SetParticleAttribute( att );
	//}
	//m_LastMouseX = m_rInputMan.GetMousePos().x;
	//m_LastMouseY = m_rInputMan.GetMousePos().y;

	//grCParticleAttributePB att = m_pPartEmitter_2->GetParticleAttribute();
	//float deg = grMath::VecToDeg( att.Velocity );
	//deg += 1.0f * deltaT;
	//grMath::RotatePoint( &att.Velocity, deg * grMath::DegToRad );
	//m_pPartEmitter_2->SetParticleAttribute( att );




	// Scenegraph things
	if ( m_rInputMan.GetMouseDown( sf::Mouse::Left ) )
	{
		//m_pPlayer->ReleaseChildByIdx( 0 );		
		//grEntityManager::Instance().DestroyEntity( m_pPlayer );
		//grEntityManager::Instance().DestroyEntity( m_pEnemy );

		m_bFlipFlop = !m_bFlipFlop;
		//m_pEnemy->SetEnable( hej );
		//m_pPlayer->SetEnable( hej );

		( m_bFlipFlop == false ) ? m_pPlayer->ReleaseChildById( m_pEnemy->GetId() ) : m_pPlayer->AddChild( m_pEnemy );
	}

	// Navmesh things
	if ( m_pMap != nullptr )
	{
		//grCore& rCore = grCore::Instance();
		//if ( m_rInputMan.GetMouse( sf::Mouse::Button::Left ) )
		//{
		//	grV2f mousePos = grV2f( ( float )sf::Mouse::getPosition( rCore.GetRenderWin() ).x, ( float )sf::Mouse::getPosition( rCore.GetRenderWin() ).y );
		//	m_pActor->SetEnd( m_pMap, mousePos );
		//	m_pActor->FindPath( m_pMap );
		//}

		//if ( m_rInputMan.GetMouse( sf::Mouse::Button::Right ) )
		//{
		//	grV2f mousePos = grV2f( ( float )sf::Mouse::getPosition( rCore.GetRenderWin() ).x, ( float )sf::Mouse::getPosition( rCore.GetRenderWin() ).y );
		//	m_pActor->SetStart( m_pMap, mousePos );
		//	m_pActor->FindPath( m_pMap );
		//}
	}
}


// Render
//////////////////////////////////////////////////
void
grSandbox::Render( sf::RenderWindow& rRenderWin )
{

}
