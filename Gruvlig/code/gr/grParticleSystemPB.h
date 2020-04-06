#ifndef		_H_GRPARTICLESYSTEMPB_
#define		_H_GRPARTICLESYSTEMPB_

#include	"grCommon.h"
#include	"grParticlePB.h"
#include	"grV2.h"


// grParticleSystemPB
//////////////////////////////////////////////////
class grParticleSystemPB
{
public:

	grParticleSystemPB( const uInt uniqueId );

	//////////////////////////////////////////////////

	const uInt GetSystemId( void ) const { return m_Id; }

	//////////////////////////////////////////////////

private:

	void Update( const float deltaT, grParticlePB& rParticle );

	//////////////////////////////////////////////////

	grV2f			m_Position,
					m_Direction,
					m_Gravity;

	uInt			m_Id;

};

#endif	// 