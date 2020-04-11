#ifndef		_H_GRPARTICLESYSTEMPB_
#define		_H_GRPARTICLESYSTEMPB_

#include	"grCommon.h"

struct		grParticleSetupPB;
class		grRandom;


// grParticleSystemPB
//////////////////////////////////////////////////
struct grParticleSystemPB
{
	grParticleSystemPB( void );
	~grParticleSystemPB( void );

	//////////////////////////////////////////////////

	void Activate( grParticleSetupPB& rParticleSetup, const float fixedT );
	void Update( grParticleSetupPB& rParticleSetup, const float fixedT );
	void Deactivate( grParticleSetupPB& rParticleSetup );

	//////////////////////////////////////////////////

	uPtr<grRandom> pRand;
};

#endif	// 