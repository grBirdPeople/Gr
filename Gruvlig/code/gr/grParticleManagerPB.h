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

using		arrParticleSetup = uPtr<grParticleSetupPB>[ PARTICLE_SETUPS ];
using		arrSystems = uPtr<grParticleSystemPB>[ PARTICLE_SYTEMS ];


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

	arrParticleSetup	m_arrParticleSetup;
	arrSystems			m_arrSystems;

	// TEST
	float				m_TimeStepCounter;
	// TEST

	uInt				m_SetupQuantity;

};

#endif	// _H_GRPARTICLEMANAGERPB_