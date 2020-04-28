#include	"grParticleEmitter.h"




grCParticleEmitter::grCParticleEmitter( const intU id )
	: m_uPAtt			( new grSParticleAttribute() )
	, m_SpawnRate		( 1.0f / 1000.0f )
	, m_SpawnTimer		( 0.0f )
	, m_Id				( id )
	, m_PartActive		( 0 )
{}


grCParticleEmitter::~grCParticleEmitter( void )
{
	m_uPAtt.reset();
}
