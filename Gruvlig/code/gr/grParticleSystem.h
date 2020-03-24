#ifndef		_H_GRPARTICLESYSTEM_
#define		_H_GRPARTICLESYSTEM_

#define		PARTICLE_QUANTITY	500

#include	"grCommon.h"
#include	"grV2.h"

class		grRandom;


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

	inline	void	SetPosition			( const grV2f& rPos )
										{
											m_ParticleBlueprint.Position = rPos;
										}
	inline	void	SetDirection		( const grV2f& direction, const float deviationInDeg = 0.0f )
										{
											m_ParticleBlueprint.Direction = direction;								// TODO: Sign it
											m_bDirRandomDiviation = deviationInDeg != 0.0f ? true : false;
											if ( m_bDirRandomDiviation == true )
											{
												m_HalfDirDiviationInDeg = deviationInDeg * 0.5f;					// TODO: Check min/max 360
											}
										}
	inline	void	SetGravity			( const grV2f& rGravity, const float randomRange = 0.0f )
										{
											m_ParticleBlueprint.Gravity = rGravity;
											m_bGravityRandomRange = randomRange != 0.0f ? true : false;
										}

	//////////////////////////////////////////////////

			void	Init				( const grV2f& position, const grV2f& direction, const float velocity, const float lifetime, const float m_SpawnsPerSec );
			void	Update				( const float deltaT );

	//////////////////////////////////////////////////

private:

	void			DeactivateParticle	( void );
	void			ActivateParticle	( const float deltaT );
	void			UpdateParticle		( const float deltaT );

	//////////////////////////////////////////////////

	std::vector<Particle*>	m_VecParticles;

	Particle				m_ParticleBlueprint;

	float					m_SpawnsPerSec,
							m_SpawnInMilliSec,
							m_SpawnTimer;

	float					m_HalfDirDiviationInDeg;

	uInt					m_ActiveParticles;

	bool					m_bDirRandomDiviation,
							m_bGravityRandomRange;

	grRandom*				m_pRand;

};

#endif	// _H_GRPARTICLESYSTEM_
