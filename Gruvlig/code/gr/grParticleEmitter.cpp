#include	"grParticleEmitter.h"




grCParticleEmitter::grCParticleEmitter( const intU id )
	: m_uPAtt			( new grSParticleAttribute() )
	, m_SpawnPerSec		( 1.0f )
	, m_SpawnRateMs		( 1.0f / m_SpawnPerSec )
	, m_SpawnTimerMs	( 0.0f )
	, m_Id				( id )
	, m_PartActive		( 0 )
{}


grCParticleEmitter::~grCParticleEmitter( void )
{
	m_uPAtt.reset();
}
