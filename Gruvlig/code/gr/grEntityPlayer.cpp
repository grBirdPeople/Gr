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
	m_pPartSys->Init( GetPosition(), grV2f( 0.0f, -1.0f ), 100.0f, 3.125f, 100 );
	//m_pPartSys->SetDirection( grV2f( 1.0f, 0.0f ), 360.0f );		// OK
	//m_pPartSys->SetDirectionRange( 360.0f );						// OK
	//m_pPartSys->SetRotation( 180.0f );							// OK
	//m_pPartSys->SetGravity( grV2f( 0.0f, 9.8f ), 5.0f );			// NOT IMPLEMENTED
	//m_pPartSys->SetVelocity( 100.0f );							// OK
	//m_pPartSys->SetVelocityChange( -0.75f );						// OK
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
		m_pPartSys->SetDirection( dir, m_pPartSys->GetSpread() );
	}

	if ( rInputMan.GetMouseScrollForwards() == true )
	{
		//grV2f dir = m_pPartSys->GetDirection();
		//float rad = 10.0f * grMath::DegToRad;
		//grMath::RotatePoint( &dir, rad );
		//m_pPartSys->SetDirection( dir, m_pPartSys->GetSpread() );
		//printf( "%s %g\n", "Deg: ", m_pPartSys->GetRotationInDeg() );

		//m_pPartSys->SetRotation( m_pPartSys->GetRotationInDeg() + 15.0f );
		//printf( "%s %g\n", "Deg: ", m_pPartSys->GetRotationInDeg() );

		m_pPartSys->SetSpread( m_pPartSys->GetSpread() + 10.0f );
		printf( "%s %g\n", "SpreadInDeg: ", m_pPartSys->GetSpread() );

		//m_pPartSys->SetVelocity( m_pPartSys->GetVelocity() + 10.0f );
		//printf( "%s %g\n", "Velocity: ", m_pPartSys->GetVelocity() );

		//m_pPartSys->SetSpeedRange( m_pPartSys->GetSpeedRange() + 10.0f );
		//printf( "%s %g\n", "VelocityRange: ", m_pPartSys->GetSpeedRange() );

		//m_pPartSys->SetVelocityChangeRange( m_pPartSys->GetVelocityChangeRange() + 1.0f );
		//printf( "%s %g\n", "VelocityChange: ", m_pPartSys->GetVelocityChangeRange() );
	}
	if ( rInputMan.GetMouseScrollBackwards() == true )
	{
		//grV2f dir = m_pPartSys->GetDirection();
		//float rad = -10.0f * grMath::DegToRad;
		//grMath::RotatePoint( &dir, rad );
		//m_pPartSys->SetDirection( dir, m_pPartSys->GetSpread() );
		//printf( "%s %g\n", "Deg: ", m_pPartSys->GetRotationInDeg() );

		//m_pPartSys->SetRotation( m_pPartSys->GetRotationInDeg() - 15.0f );
		//printf( "%s %g\n", "Deg: ", m_pPartSys->GetRotationInDeg() );

		m_pPartSys->SetSpread( m_pPartSys->GetSpread() - 10.0f );
		printf( "%s %g\n", "SpreadInDeg: ", m_pPartSys->GetSpread() );

		//m_pPartSys->SetVelocity( m_pPartSys->GetVelocity() - 10.0f );
		//printf( "%s %g\n", "Velocity: ", m_pPartSys->GetVelocity() );

		//m_pPartSys->SetSpeedRange( m_pPartSys->GetSpeedRange() - 10.0f );
		//printf( "%s %g\n", "VelocityRange: ", m_pPartSys->GetSpeedRange() );

		//m_pPartSys->SetVelocityChangeRange( m_pPartSys->GetVelocityChangeRange() - 1.0f );
		//printf( "%s %g\n", "VelocityChange: ", m_pPartSys->GetVelocityChangeRange() );
	}

	if ( GetChildByIdx( 1 ) != nullptr )
	{
		grIEntity* childEntTwo = GetChildByIdx( 1 )->GetChildByIdx( 0 );
		if ( childEntTwo != nullptr )
		{
			m_pPartSys->SetPosition( childEntTwo->GetPosition() );
		}

	}
	else if ( GetChildByIdx( 0 ) != nullptr )
	{
		grIEntity* childEntTwo = GetChildByIdx( 0 )->GetChildByIdx( 0 );
		if ( childEntTwo != nullptr )
		{
			m_pPartSys->SetPosition( childEntTwo->GetPosition() );
		}
	}
	// TEST
	
#ifdef DEBUG
	// Debug draw
	//grBBox box( grV2f( 10.0f, 10.0f ), GetPosition() );
	//grDebugManager::Instance( ).AddBBox( box, sf::Color::Green );
#endif // DEBUG
}