#ifndef		_H_GRPARTICLEMANAGERPB_
#define		_H_GRPARTICLEMANAGERPB_

#define		PARTICLE_SETUPS		100
#define		PARTICLE_PER_SETUP	500
#define		PARTICLE_TIMESTEP	1.0f / 60.0f
#define		PARTICLE_SYTEMS		1				// Simulate threads

#include	"grParticlePB.h"
#include	"grSingleton.h"
#include	"grParticleSystemPB.h"

struct		grParticlePB;
struct		grParticleSetupPB;

using		vecParticleSetup = std::vector<uPtr<grParticleSetupPB>>;
using		vecSystems = std::vector<uPtr<grParticleSystemPB>>;	// If I wan't to thread there can exists one system for each thread


// TODO: Setup is one particle attribute followed by all the particles per particle system instance.
// Struct holds one attribute and a vector of max particles. Structs in vector.
// Possible to have one system for each thread. System iterates over struct vector. Test if: one or three loops of particle setups Activate/Deactivate/Update.
// Avoid pointer chasing is done on Activate/Deactivate/Update.


// grParticleManagerPB
//////////////////////////////////////////////////
class grParticleManagerPB : public grSingleton<grParticleManagerPB>
{
public:

	grParticleManagerPB( void );
	~grParticleManagerPB( void );

	//////////////////////////////////////////////////

	void Init( void );

	inline grParticleAttributePB CreateParticleAttribute( void );
	grParticleSetupPB* const CreateSetup( void );	// TODO: This should take a particle attribute as parameter and return some API
	void Update( const float deltaT );

	//////////////////////////////////////////////////

private:

	vecSystems			m_vecSystems;
	vecParticleSetup	m_vecParticleSetup;

	uInt				m_SetupQuantity;

	// TEST
	float				m_TimeStepCounter;
	// TEST

};

#endif	// _H_GRPARTICLEMANAGERPB_