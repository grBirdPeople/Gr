#include	"grParticleSystemPB.h"

#include	"grCore.h"
#include	"grDefine.h"


// cTor
//////////////////////////////////////////////////
grParticleSystemPB::grParticleSystemPB( const uInt uniqueId )
	: m_Direction			( grV2f( 0.0f, -1.0f ) )
	, m_Id					( uniqueId )
	, m_Gravity				( grV2f( 0.0f, 9.8f ) )
{
	grCore& c = grCore::Instance();
	m_Position = grV2f( c.GetRenderWin().getSize().x * 0.5f, c.GetRenderWin().getSize().y * 0.5f );
}


// Update
//////////////////////////////////////////////////
void
grParticleSystemPB::Update( const float deltaT, grParticlePB& rParticle )
{

}
