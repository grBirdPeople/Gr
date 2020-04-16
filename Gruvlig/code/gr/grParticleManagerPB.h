#ifndef		_H_GRPARTICLEMANAGERPB_
#define		_H_GRPARTICLEMANAGERPB_

#define		PARTICLE_SYTEMS			1				// If threads would happen each thread can have it's own
#define		PARTICLE_EMITTERS		8				// Not really an emitter but I don't know what to call it so
#define		PARTICLE_PER_EMITTER	128

#include	"grSingleton.h"
#include	"grParticleEmitterPB.h"
#include	"grParticleSystemPB.h"


// grParticleManagerPB
//////////////////////////////////////////////////
class grCParticleManagerPB : public grSingleton<grCParticleManagerPB>
{
public:

	friend class grCore;	// Calls this update

	//////////////////////////////////////////////////

	grCParticleManagerPB( void );
	~grCParticleManagerPB( void );

	//////////////////////////////////////////////////

	grSParticleEmitter* const CreateEmitter( void );
	grSParticleEmitter* const GetEmitter( const uInt id );

	//////////////////////////////////////////////////

private:

	void Update( const float deltaT );

	//////////////////////////////////////////////////

	using vecUpEmitter = std::vector<uP<grSParticleEmitter>>;
	using vecUpSystem = std::vector<std::unique_ptr<grSParticleSystemPB>>;

	vecUpEmitter	m_VecUpEmitter;
	vecUpSystem		m_VecUpSystem;

	uInt	m_CreatedSystems,
			m_CreatedEmitters,
			m_TotalParticles;
};

#endif	// _H_GRPARTICLEMANAGERPB_