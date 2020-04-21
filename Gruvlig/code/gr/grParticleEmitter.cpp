#include	"grParticleEmitter.h"




grCParticleEmitter::grCParticleEmitter( const uInt id )
	: m_uPAtt			( new grSParticleAttribute() )
	, m_Position		( grV2f() )
	, m_SpawnRate		( 1.0f / 75.0f )
	, m_SpawnTimer		( 0.0f )
	, m_Id				( id )
	, m_PartActive		( 0 )
	, m_DataModified	( false )
{
	m_uPAtt->Position = m_Position;
}


grCParticleEmitter::~grCParticleEmitter( void )
{
	m_uPAtt.reset();
}
