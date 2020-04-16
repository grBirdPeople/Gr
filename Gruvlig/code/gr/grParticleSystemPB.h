#ifndef		_H_GRPARTICLESYSTEMPB_
#define		_H_GRPARTICLESYSTEMPB_

#include	"grCommon.h"

class		grCParticleEmitter;
class		grRandom;



// grSParticleSystemPB
//////////////////////////////////////////////////
struct grSParticleSystemPB
{
	grSParticleSystemPB( const uInt id, const uInt partQuantity );
	~grSParticleSystemPB( void );

	grSParticleSystemPB( grSParticleSystemPB const& ) = delete;
	grSParticleSystemPB& operator=( grSParticleSystemPB  const& ) = delete;

	//////////////////////////////////////////////////

	void Activate( grCParticleEmitter& rEmitter, const float deltaT );
	void Update( grCParticleEmitter& rEmitter, const float deltaT );
	void Deactivate( grCParticleEmitter& rEmitter );

	//////////////////////////////////////////////////

private:

	// As particles lifetimes can end in an non linear manner relative their memory adress, their idx get's stored here then sorted  at a later time and deactivated linearly
	// Unsure if this saves time, perhaps if the particle quantity is large enough
	// Might be handy anyway, unless it worsens performance, where it's easy to cut out
	uP<grRandom>	uPRand;
	uInt			Id;
};

#endif	// 