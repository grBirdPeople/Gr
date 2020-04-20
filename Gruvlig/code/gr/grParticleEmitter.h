#ifndef		_H_GRPARTICLE_EMITTER_
#define		_H_GRPARTICLE_EMITTER_

#include	"grCommon.h"
#include	"grV2.h"

#include	"grSPartAttribute.h"


class grCParticleEmitter
{
public:

	friend class grCParticleManager;
	friend class grCParticleSystem;

	grCParticleEmitter( const uInt id );
	~grCParticleEmitter( void );

	grCParticleEmitter( const grCParticleEmitter& ) = delete;
	grCParticleEmitter& operator=( const grCParticleEmitter& ) = delete;

	inline const uInt GetId( void ) const
	{
		return m_Id;
	}
	inline void SetPosition( const grV2f& rPos )
	{
		m_uPAtt->Position = rPos;
		m_Position = rPos;
		m_DataModified = true;
	}

private:

	uP<grSParticleAttribute>	m_uPAtt;

	grV2f	m_Position;

	float	m_SpawnRate,
			m_SpawnTimer;

	uInt	m_Id,
			m_PartActive;

	bool	m_DataModified;
};

#endif	// _H_GRPARTICLE_EMITTER_


