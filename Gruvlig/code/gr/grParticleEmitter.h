#ifndef		_H_GRPARTICLE_EMITTER_
#define		_H_GRPARTICLE_EMITTER_

#include	"grCommon.h"
#include	"grV2.h"

struct	grSParticleAttribute;


class grCParticleEmitter
{
public:

	friend class grCParticleManager;
	friend class grCParticleSystem;

	grCParticleEmitter( const uInt id );
	~grCParticleEmitter( void );

	grCParticleEmitter( const grCParticleEmitter& ) = delete;
	grCParticleEmitter& operator=( const grCParticleEmitter& ) = delete;

	const uInt GetId( void )
	{
		return m_Id;
	}

private:

	uP<grSParticleAttribute>	m_uPAtt;

	grV2f	m_Position;

	float	m_SpawnRate,
			m_SpawnTimer;

	uInt	m_Id,			// Is set when user calls create on the manager
			m_PartActive;
};

#endif	// _H_GRPARTICLE_EMITTER_


