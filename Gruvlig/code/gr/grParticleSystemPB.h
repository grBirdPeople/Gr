#ifndef		_H_GRPARTICLESYSTEMPB_
#define		_H_GRPARTICLESYSTEMPB_

struct		grParticleSetupPB;


// grParticleSystemPB
//////////////////////////////////////////////////
struct grParticleSystemPB
{
	void Update( grParticleSetupPB& rParticleSetup, const float fixedT );
	void Deactivate( grParticleSetupPB& rParticleSetup );
	void Activate( grParticleSetupPB& rParticleSetup, const float fixedT );
};

#endif	// 