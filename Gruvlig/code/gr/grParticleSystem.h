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
			Velocity		= 0.0f;
			VelocityChange	= 0.0f;
			LifeTime		= 0.0f;
		}

		// TODO: Would be cleaner to add rands here instead of in class... but particles would get heavier... Unsure of what's best
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

	inline	void	SetPosition			( const grV2f& rPos, const grV2f& randomRange = grV2f() )
										{
											m_ParticleBlueprint.Position = rPos;
											if ( randomRange.x != 0.0f || randomRange.y != 0.0f )
											{
												m_bPosRandRange = true;
												m_RandPosRange = randomRange;
												m_RandPosRange *= 0.5f;
											}
										}
	inline	void	SetDirection		( const grV2f& direction, const float deviationInDeg = 0.0f )
										{
											m_ParticleBlueprint.Direction = direction;							// TODO: Sign it
											m_bDirRandDiviation = deviationInDeg != 0.0f ? true : false;
											if ( m_bDirRandDiviation == true )
											{
												m_RandDirDiviationInDeg = deviationInDeg * 0.5f;				// TODO: Check min/max 360
											}
										}
	inline	void	SetGravity			( const grV2f& rGravity, const float randomRange = 0.0f )
										{
											m_ParticleBlueprint.Gravity = rGravity;
											m_bGravityRandRange = randomRange != 0.0f ? true : false;
											if ( m_bGravityRandRange == true )
											{
												m_RandGravityRange = randomRange * 0.5f;
											}
										}
	inline	void	SetVelocity			( const float velocity, const float randomRange = 0.0f )
										{
											m_ParticleBlueprint.Velocity = velocity;
											m_bVelocityRandRange = ( randomRange  != 0.0f ) ? true : false;
											if ( m_bVelocityRandRange == true )
											{
												m_RandVelocityRange = randomRange * 0.5f;
											}
										}
	inline	void	SetVelocityChange	( const float velocityChange, const float randomRange = 0.0f )
										{
											m_ParticleBlueprint.VelocityChange = velocityChange;
											float range = ( randomRange < 0.0f ) ? 0.0f : randomRange;
											m_bVelocityChangeRandRange = ( range > 0.0f ) ? true : false;
											if ( m_bVelocityChangeRandRange == true )
											{
												m_RandVelocityChangeRange = range * 0.5f;
											}
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

	grV2f					m_RandPosRange;		// TODO: Implement this

	float					m_SpawnsPerSec,
							m_SpawnInMilliSec,
							m_SpawnTimer;

	float					m_RandDirDiviationInDeg,
							m_RandGravityRange,
							m_RandVelocityRange,
							m_RandVelocityChangeRange;

	uInt					m_ActiveParticles;

	bool					m_bPosRandRange,
							m_bDirRandDiviation,
							m_bGravityRandRange,
							m_bVelocityRandRange,
							m_bVelocityChangeRandRange;

	grRandom*				m_pRand;

};

#endif	// _H_GRPARTICLESYSTEM_
