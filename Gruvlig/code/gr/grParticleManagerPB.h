#ifndef		_H_GRPARTICLEMANAGERPB_
#define		_H_GRPARTICLEMANAGERPB_

#define		PARTICLE_SYS		100
#define		PARTICLE_PER_SYS	500
#define		PARTICLE_TIMESTEP	1.0f / 120.0f

#include	"grParticlePB.h"
#include	"grSingleton.h"

struct		grParticlePB;
struct		grParticleSetupPB;
struct		grParticleSystemPB;

using		vecParticle = std::vector<uPtr<grParticlePB>>;
using		vecParticleSetup = std::vector<uPtr<grParticleSetupPB>>;


// TODO: Setup is one particle attribute followed by all the particles per particle system instance.
// Struct holds one attribute and a vector of max particles. Structs in vector.
// One system exists. System iterates over struct vector. Test if: one or three loops of particle setups Activate/Deactivate/Update.
// Make sure no pointer chasing is done on Activate/Deactivate/Update.


// grParticleManagerPB
//////////////////////////////////////////////////
class grParticleManagerPB : public grSingleton<grParticleManagerPB>
{
public:

	grParticleManagerPB( void );
	~grParticleManagerPB( void );

	//////////////////////////////////////////////////



	//////////////////////////////////////////////////

	void Init( void );

	// grParticleAttributePB* CreateParticleAttribute( void );	// TODO: Implement.
	grParticleSetupPB* const CreateSystem( void );	// TODO: This should take a particle attribute as parameter and return some API
	void Update( const float deltaT );

	//////////////////////////////////////////////////

private:

	vecParticleSetup			m_VecParticleSetup;
	uPtr<grParticleSystemPB>	m_pParticleSystem;

	uInt						m_SetupQuantity;

	// TEST
	float						m_TimeStepCounter;
	// TEST

};


// grParticleSetupPB
//////////////////////////////////////////////////
struct grParticleSetupPB
{
	grParticleSetupPB( void )
		: pParticleAttribute	( nullptr )
		, SpawnInMilliSec		( 1.0f / 3.0f )
		, SpawnCounter			( SpawnInMilliSec )
		, Id					( 0 )
		, ParticlesActive		( 0 )
		
	{
		VecParticle.reserve( PARTICLE_PER_SYS );
	}

	uPtr<grParticleAttributePB>	pParticleAttribute;
	vecParticle					VecParticle;
	float						SpawnInMilliSec,
								SpawnCounter;
	uInt						Id,
								ParticlesActive;
};

#endif	// _H_GRPARTICLEMANAGERPB_