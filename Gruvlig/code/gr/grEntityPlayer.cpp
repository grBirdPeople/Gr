#include	"grEntityPlayer.h"

#include	"grBBox.h"
#include	"grDebugManager.h"
#include	"grDefine.h"
#include	"grInput.h"
#include	"grCtrlPlayer.h"


// cTor
//////////////////////////////////////////////////
grEntityPlayer::grEntityPlayer( const grIEntity::EEntityType type, const str& rName, const grV2f& rPos, const uInt id )
	: m_pCtrl	( new grCtrlPlayer( this ) )
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
	grBBox box( grV2f( 10.0f, 10.0f ), GetPosition() );
	grDebugManager::Instance().AddBBox( box, sf::Color::Green );
#endif // DEBUG
}