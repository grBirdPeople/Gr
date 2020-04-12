#ifndef		_H_GRPARTICLESYSTEMPB_
#define		_H_GRPARTICLESYSTEMPB_

#include	"grCommon.h"

struct		grParticleSetupPB;
class		grRandom;

// All alive particles are aligned so no iteration over dead/deactivated particles are done.
// When a particle dies it's data get's overwritten from the last alive particle in the list and the list count get's set to one less.
// See deactivate function.

// grParticleSystemPB
//////////////////////////////////////////////////
struct grParticleSystemPB
{
	grParticleSystemPB( void );
	~grParticleSystemPB( void );

	grParticleSystemPB( grParticleSystemPB const& ) = delete;
	grParticleSystemPB& operator=( grParticleSystemPB  const& ) = delete;

	//////////////////////////////////////////////////

	void Activate( grParticleSetupPB& rParticleSetup, const float fixedT );
	void Update( grParticleSetupPB& rParticleSetup, const float fixedT );
	void Deactivate( grParticleSetupPB& rParticleSetup );

	//////////////////////////////////////////////////

	uPtr<grRandom> pRand;
};

#endif	// 