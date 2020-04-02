#include	"grEntityPlayer.h"

#include	"grDebugManager.h"
#include	"grInputManager.h"
#include	"grCtrlPlayer.h"

// TEST
#include	"grParticleManager.h"
#include	"grParticleSystem.h"
// TEST


// cTor
//////////////////////////////////////////////////
grEntityPlayer::grEntityPlayer( const grEnums::EntityType type, const str& rName, const grV2f& rPos, const uInt id )
	: m_pCtrl	( new grCtrlPlayer( this ) )
{
	m_PosWorld		= rPos;
	m_PosLocal		= rPos;
	m_Name			= rName;
	m_Id			= id;
	m_bIsEnabled	= true;

	SetType( type );

	// TEST
	m_pPartSys = grParticleManager::Instance().CreateParticleSystem();
	m_pPartSys->Init( GetPosition(), grV2f( 0.0f, -1.0f ), 100.0f, 6.0f, 100 );
	//m_pPartSys->SetDirection( grV2f( 1.0f, 0.0f ), 360.0f );		// OK
	//m_pPartSys->SetDirectionRange( 360.0f );						// OK
	//m_pPartSys->SetRotation( 180.0f );							// OK
	m_pPartSys->SetGravity( grV2f( 0.0f, 1.0f ), 5.0f );			// NOT IMPLEMENTED
	//m_pPartSys->SetSpeed( 0.0f );									// OK
	//m_pPartSys->SetSpeedChange( -0.75f );							// OK
	// TEST
}


// dTor
//////////////////////////////////////////////////
grEntityPlayer::~grEntityPlayer( void )
{
	if ( m_pCtrl != nullptr )
	{
		DELANDNULL( m_pCtrl );
	}
}


// Update
//////////////////////////////////////////////////
void
grEntityPlayer::Update( const float deltaT )
{
	grIEntity::Update();

	// Controller
	m_pCtrl->Update();
	
	// Position
	grV2f newPos = m_PosWorld + m_pCtrl->GetInputDir() * m_SpdWalk * deltaT;
	if ( newPos != GetPosition() )
	{
		SetPosition( newPos );
	}

	// TEST
	grInputManager& rInputMan = grInputManager::Instance();

	if ( rInputMan.GetMouseMoved() == true )
	{
		grV2f dir = ( rInputMan.GetMousePos() - m_pPartSys->GetPosition() ).Normalized();
		m_pPartSys->SetDirection( dir, m_pPartSys->GetDirectionRange() );
	}

	if ( rInputMan.GetMouseScrollForwards() == true )
	{
		//grV2f range;
		//range.x = m_pPartSys->GetPositionRange().x;
		//range.y = m_pPartSys->GetPositionRange().y;
		//range += 5.0f;
		//m_pPartSys->SetPositionRange( range );
		//printf( "%s %g \t %g \n", "Pos: ", m_pPartSys->GetPositionRange().x, m_pPartSys->GetPositionRange().y );

		//grV2f dir = m_pPartSys->GetDirection();
		//float rad = 10.0f * grMath::DegToRad;
		//grMath::RotatePoint( &dir, rad );
		//m_pPartSys->SetDirection( dir, m_pPartSys->GetSpread() );
		//printf( "%s %g\n", "Deg: ", m_pPartSys->GetRotationInDeg() );

		//m_pPartSys->SetRotation( m_pPartSys->GetRotationInDeg() + 15.0f );
		//printf( "%s %g\n", "Deg: ", m_pPartSys->GetRotationInDeg() );

		//m_pPartSys->SetLifetime( m_pPartSys->GetLifetime() + 0.5f );
		//printf( "%s %g\n", "Lifetime: ", m_pPartSys->GetLifetime() );

		//m_pPartSys->SetLifetimeRange( m_pPartSys->GetLifetimeRange() + 0.25f );
		//printf( "%s %g\n", "LifetimeRange: ", m_pPartSys->GetLifetimeRange() );

		//m_pPartSys->SetDirectionRange( m_pPartSys->GetDirectionRange() + 10.0f );
		//printf( "%s %g\n", "SpreadInDeg: ", m_pPartSys->GetDirectionRange() );

		m_pPartSys->SetGravityRange( m_pPartSys->GetGravityRange() + 5.0f );
		printf( "%s %g\n", "Gravity X: ", m_pPartSys->GetGravityRange() );

		//m_pPartSys->SetSpeed( m_pPartSys->GetSpeed() + 10.0f );
		//printf( "%s %g\n", "Speed: ", m_pPartSys->GetSpeed() );
		
		//m_pPartSys->SetSpeedRange( m_pPartSys->GetSpeedRange() + 10.0f );
		//printf( "%s %g\n", "VelocityRange: ", m_pPartSys->GetSpeedRange() );

		//m_pPartSys->SetSpeedChangeRange( m_pPartSys->GetSpeedChangeRange() + 0.25f );
		//printf( "%s %g\n", "SpeedChange: ", m_pPartSys->GetSpeedChangeRange() );
	}
	if ( rInputMan.GetMouseScrollBackwards() == true )
	{
		//grV2f range;
		//range.x = m_pPartSys->GetPositionRange().x;
		//range.y = m_pPartSys->GetPositionRange().y;
		//range -= 5.0f;
		//m_pPartSys->SetPositionRange( range );
		//printf( "%s %g \t %g \n", "Pos: ", m_pPartSys->GetPositionRange().x, m_pPartSys->GetPositionRange().y );

		//grV2f dir = m_pPartSys->GetDirection();
		//float rad = -10.0f * grMath::DegToRad;
		//grMath::RotatePoint( &dir, rad );
		//m_pPartSys->SetDirection( dir, m_pPartSys->GetSpread() );
		//printf( "%s %g\n", "Deg: ", m_pPartSys->GetRotationInDeg() );

		//m_pPartSys->SetRotation( m_pPartSys->GetRotationInDeg() - 15.0f );
		//printf( "%s %g\n", "Deg: ", m_pPartSys->GetRotationInDeg() );

		//m_pPartSys->SetLifetime( m_pPartSys->GetLifetime() - 0.5f );
		//printf( "%s %g\n", "Lifetime: ", m_pPartSys->GetLifetime() );

		//m_pPartSys->SetLifetimeRange( m_pPartSys->GetLifetimeRange() - 0.25f );
		//printf( "%s %g\n", "LifetimeRange: ", m_pPartSys->GetLifetimeRange() );

		//m_pPartSys->SetDirectionRange( m_pPartSys->GetDirectionRange() - 10.0f );
		//printf( "%s %g\n", "SpreadInDeg: ", m_pPartSys->GetDirectionRange() );

		m_pPartSys->SetGravityRange( m_pPartSys->GetGravityRange() - 5.0f );
		printf( "%s %g\n", "Gravity X: ", m_pPartSys->GetGravityRange() );

		//m_pPartSys->SetSpeed( m_pPartSys->GetSpeed() - 10.0f );
		//printf( "%s %g\n", "Speed: ", m_pPartSys->GetSpeed() );

		//m_pPartSys->SetSpeedRange( m_pPartSys->GetSpeedRange() - 10.0f );
		//printf( "%s %g\n", "VelocityRange: ", m_pPartSys->GetSpeedRange() );

		//m_pPartSys->SetSpeedChangeRange( m_pPartSys->GetSpeedChangeRange() - 0.25f );
		//printf( "%s %g\n", "SpeedChange: ", m_pPartSys->GetSpeedChangeRange() );
	}

	if ( GetChildByIdx( 1 ) != nullptr )
	{
		grIEntity* childEntTwo = GetChildByIdx( 1 )->GetChildByIdx( 0 );
		if ( childEntTwo != nullptr )
		{
			//m_pPartSys->SetPosition( childEntTwo->GetPosition(), m_pPartSys->GetPositionRange() );
		}

	}
	else if ( GetChildByIdx( 0 ) != nullptr )
	{
		grIEntity* childEntTwo = GetChildByIdx( 0 )->GetChildByIdx( 0 );
		if ( childEntTwo != nullptr )
		{
			//m_pPartSys->SetPosition( childEntTwo->GetPosition(), m_pPartSys->GetPositionRange() );
		}
	}
	// TEST
	
#ifdef DEBUG
	// Debug draw
	//grBBox box( grV2f( 10.0f, 10.0f ), GetPosition() );
	//grDebugManager::Instance( ).AddBBox( box, sf::Color::Green );
#endif // DEBUG
}