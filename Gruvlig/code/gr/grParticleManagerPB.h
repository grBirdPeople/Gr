#ifndef		_H_GRPARTICLEMANAGERPB_
#define		_H_GRPARTICLEMANAGERPB_

#define		PARTICLE_SYTEMS			1				// If threads would happen each thread can have it's own
#define		PARTICLE_EMITTERS		8				// Not really an emitter but I don't know what to call it so
#define		PARTICLE_x_EMITTER	128
//#define		PARTICLE_TIMESTEP		1.0f / 60.0f	// If physics update for particles

#include	"grSingleton.h"
#include	"grParticlePB.h"
#include	"grParticleSystemPB.h"


// grParticleManagerPB
//////////////////////////////////////////////////
class grParticleManagerPB : public grSingleton<grParticleManagerPB>
{
public:

	friend class grCore;	// Calls this update

	//////////////////////////////////////////////////

	grParticleManagerPB( void );
	~grParticleManagerPB( void );

	//////////////////////////////////////////////////

	const sInt Create( void );
	grParticlAttributePB Get( const uInt id );
	void Set( const sInt id, const grParticlAttributePB& rAtt );

	//////////////////////////////////////////////////

private:

	void Update( const float deltaT );

	//////////////////////////////////////////////////

	uP<uP<SParticleEmitter>[]>		m_uPArrEmitters;
	uP<uP<grParticleSystemPB>[]>	m_uPSystems;

	uP<grStruct::grLoopQue<grParticlAttributePB>>	m_AttributeQue;	// If this is should work it needs to be an array, one que for each emitter created. Also probably should 

	uInt	m_CreatedSystems,
			m_CreatedEmitters,
			m_TotalParticles;
};

#endif	// _H_GRPARTICLEMANAGERPB_