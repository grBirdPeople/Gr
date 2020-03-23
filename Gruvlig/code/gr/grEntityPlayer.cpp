#include	"grEntityPlayer.h"

#include	"grDebugManager.h"
#include	"grInput.h"
#include	"grCtrlPlayer.h"


// cTor
//////////////////////////////////////////////////
grEntityPlayer::grEntityPlayer( const grEnums::EntityType type, const str& rName, const grV2f& rPos, const uInt id )
	: m_pCtrl	( new grCtrlPlayer( this ) )
	, m_pBBox	( new grBBox( grV2f( 10.0f ,10.0f ), rPos ) )
{
	m_PosWorld		= rPos;
	m_PosLocal		= rPos;
	m_Name			= rName;
	m_Id			= id;
	m_bIsEnabled	= true;

	SetType( type );
}


// dTor
//////////////////////////////////////////////////
grEntityPlayer::~grEntityPlayer( void )
{
	if ( m_pCtrl != nullptr )
	{
		DELANDNULL( m_pCtrl );
	}

	if ( m_pBBox != nullptr )
	{
		DELANDNULL( m_pBBox );
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
	
#ifdef DEBUG
	// Debug draw

	m_pBBox->SetDimensions( grV2f( 20.0f, 20.0f ));
	m_pBBox->SetPosition( GetPosition() );
	grDebugManager::Instance( ).AddBBox( *m_pBBox, sf::Color::Green );

	//grBBox box( grV2f( 10.0f, 10.0f ), GetPosition() );
	//grDebugManager::Instance( ).AddBBox( box, sf::Color::Green );
#endif // DEBUG
}