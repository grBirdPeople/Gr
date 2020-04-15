#ifndef		_H_GRPARTICLESYSTEMPB_
#define		_H_GRPARTICLESYSTEMPB_

#include	"grCommon.h"

struct		grParticlAttributePB;
struct		grParticlePB;
class		grRandom;

struct		SParticleEmitter;


// grParticleSystemPB
//////////////////////////////////////////////////
struct grParticleSystemPB
{
	grParticleSystemPB( const uInt id, const uInt partQuantity );
	~grParticleSystemPB( void );

	grParticleSystemPB( grParticleSystemPB const& ) = delete;
	grParticleSystemPB& operator=( grParticleSystemPB  const& ) = delete;

	//////////////////////////////////////////////////

	void Activate( SParticleEmitter& rPartBlock, const float deltaT );
	void Update( SParticleEmitter& rPartBlock, const float deltaT );
	void Deactivate( SParticleEmitter& rPartBlock );

	//////////////////////////////////////////////////

private:

	uP<uInt[]>	uPArrDeactivateId;

	uP<grRandom>	uPRand;

	uInt	Id;
};

#endif	// 