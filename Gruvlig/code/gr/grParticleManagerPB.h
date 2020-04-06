#ifndef		_H_GRPARTICLEMANAGERPB_
#define		_H_GRPARTICLEMANAGERPB_

#define		MAX_PARTICLE_SYS	100
#define		PARTICLES_PER_SYS	500

#include	"grParticleSystemPB.h"
#include	"grSingleton.h"

typedef		std::vector<uInt>						vecId;
typedef		std::vector<grParticleSystemPB*>		vecParticleSystems;
typedef		std::vector<std::vector<grParticlePB*>>	vecParticles;		// First vector idx is particle system id


// grParticleManagerPB
//////////////////////////////////////////////////
class grParticleManagerPB : public grSingleton<grParticleManagerPB>
{
public:

	grParticleManagerPB( void );
	~grParticleManagerPB( void );

	//////////////////////////////////////////////////

	void SetSystemQantity( const uInt quantity );

	//////////////////////////////////////////////////

	grParticleSystemPB* const CreateSystem( void );
	void Update( const float deltaT );

	//////////////////////////////////////////////////

private:

	vecParticles		m_VecParticles;
	vecParticleSystems	m_VecSystems;
	vecId				m_VecId;

	uInt				m_SystemQuantity;

};

#endif	// _H_GRPARTICLEMANAGERPB_