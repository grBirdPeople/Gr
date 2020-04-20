#include	"grParticleEmitter.h"

#include	"grSPartAttribute.h"


grCParticleEmitter::grCParticleEmitter( const uInt id )
	: m_uPAtt		( new grSParticleAttribute() )
	, m_Position	( grV2f( 320.0f, 180.0f ) )
	, m_SpawnRate	( 1.0f / 8.0f )
	, m_SpawnTimer	( 0.0f )
	, m_Id			( id )
	, m_PartActive	( 0 )
{

}


grCParticleEmitter::~grCParticleEmitter( void )
{
	m_uPAtt.reset();
}
