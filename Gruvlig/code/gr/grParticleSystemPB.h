#ifndef		_H_GRPARTICLESYSTEMPB_
#define		_H_GRPARTICLESYSTEMPB_

#include	"grCommon.h"

class		grCParticleEmitterPB;
class		grRandom;



// grSParticleSystemPB
//////////////////////////////////////////////////
struct grSParticleSystemPB
{
	grSParticleSystemPB( const intU id, const intU partQuantity );
	~grSParticleSystemPB( void );

	grSParticleSystemPB( grSParticleSystemPB const& ) = delete;
	grSParticleSystemPB& operator=( grSParticleSystemPB  const& ) = delete;

	//////////////////////////////////////////////////

	void Activate( grCParticleEmitterPB& rEmitter, const float deltaT );
	void Update( grCParticleEmitterPB& rEmitter, const float deltaT );
	void Deactivate( grCParticleEmitterPB& rEmitter );

	//////////////////////////////////////////////////

private:

	// As particles lifetimes can end in an non linear manner relative their memory adress, their idx get's stored here then sorted  at a later time and deactivated linearly
	// Unsure if this saves time, perhaps if the particle quantity is large enough
	// Might be handy anyway, unless it worsens performance, where it's easy to cut out
	pU<grRandom>	uPRand;
	intU			Id;
};

#endif	// 