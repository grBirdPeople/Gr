#ifndef		_H_GRPARTICLESYSTEMPB_
#define		_H_GRPARTICLESYSTEMPB_

#include	"grCommon.h"

struct		grParticlAttributePB;
struct		grParticlePB;
class		grRandom;

struct		SParticleBlock;


// grParticleSystemPB
//////////////////////////////////////////////////
struct grParticleSystemPB
{
	grParticleSystemPB( const uInt id );
	~grParticleSystemPB( void );

	grParticleSystemPB( grParticleSystemPB const& ) = delete;
	grParticleSystemPB& operator=( grParticleSystemPB  const& ) = delete;

	//////////////////////////////////////////////////

	void Activate( uPtr<SParticleBlock>& rPartBlock, const float deltaT );
	void Update( uPtr<SParticleBlock>& rPartBlock, const float deltaT );
	void Deactivate( uPtr<SParticleBlock>& rPartBlock );

	//////////////////////////////////////////////////

private:

	uPtr<grRandom>	pRand;

	uInt			Id;
};

#endif	// 