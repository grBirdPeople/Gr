#ifndef		_H_GRPARTICLESYSTEM_
#define		_H_GRPARTICLESYSTEM_

#define		PARTICLE_QUANTITY	100

#include	"grCommon.h"
#include	"grV2.h"


// grParticleSystem
//////////////////////////////////////////////////
class grParticleSystem
{
public:

	struct Particle
	{
		Particle( void )
		{
			Position		= grV2f();
			Direction		= grV2f();
			Gravity			= grV2f();
			Velocity		= 0.0f;
			VelocityChange	= 0.0f;
			LifeTime		= 0.0f;
		}

		grV2f	Position;
		grV2f	Direction;
		grV2f	Gravity;
		float	Velocity;
		float	VelocityChange;
		float	LifeTime;
	};

	//////////////////////////////////////////////////

				grParticleSystem	( void );
				~grParticleSystem	( void );

	//////////////////////////////////////////////////

	void		Init				( const grV2f& position, const grV2f& direction, const float velocity, const float lifetime, const float m_SpawnsPerSec );
	void		Update				( const float deltaT );

	//////////////////////////////////////////////////

private:

	void		DeactivateParticle	( void );
	void		ActivateParticle	( const float deltaT );
	void		UpdateParticle		( const float deltaT );

	//////////////////////////////////////////////////

	std::vector<Particle*>	m_VecParticles;

	Particle				m_ParticleBlueprint;

	float					m_SpawnsPerSec,
							m_SpawnInMilliSec,
							m_SpawnTimer;

	uInt					m_ActiveParticles;

};

#endif	// _H_GRPARTICLESYSTEM_
