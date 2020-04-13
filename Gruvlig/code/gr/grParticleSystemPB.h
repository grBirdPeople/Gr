#ifndef		_H_GRPARTICLESYSTEMPB_
#define		_H_GRPARTICLESYSTEMPB_

#include	"grCommon.h"

struct		grParticleAttributePB;
struct		grParticlePB;
class		grRandom;


// grParticleSystemPB
//////////////////////////////////////////////////
struct grParticleSystemPB
{
	grParticleSystemPB( const uInt id );
	~grParticleSystemPB( void );

	grParticleSystemPB( grParticleSystemPB const& ) = delete;
	grParticleSystemPB& operator=( grParticleSystemPB  const& ) = delete;

	//////////////////////////////////////////////////

	const uInt GetId( void ) const { return Id; }

	//////////////////////////////////////////////////

	void Activate( uPtr<grParticlePB*[]>& rArrPart,
				   const uPtr<grParticleAttributePB>& rArrAtt,
				   const float deltaT );

	void Update( uPtr<grParticlePB*[]>& rArrPart,
				 const uPtr<grParticleAttributePB>& rArrAtt,
				 const float deltaT );

	void Deactivate( uPtr<grParticlePB*[]>& rArrPart,
					 const uPtr<grParticleAttributePB>& rArrAtt );

	//////////////////////////////////////////////////

private:

	uPtr<grRandom>	pRand;

	uInt			Id;
};

#endif	// 