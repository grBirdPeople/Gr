#include	"grEntityEnemy.h"

#include	"grDebugManager.h"


// cTor
//////////////////////////////////////////////////
grEntityEnemy::grEntityEnemy( const grEnums::EntityType type, const str& rName, const grV2f& rPos, const intU id )
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
grEntityEnemy::~grEntityEnemy( void )	{}


// Update
//////////////////////////////////////////////////
void
grEntityEnemy::Update( const float deltaT )
{
	grIEntity::Update();

#ifdef DEBUG
	grBBox box( grV2f( 10.0f, 10.0f ), GetPosition() );
	grDebugManager::Instance().AddBBox( box, sf::Color::Magenta );
#endif // DEBUG
}
