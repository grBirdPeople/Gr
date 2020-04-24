#ifndef		_H_GRCPARTICLEMANAGER_
#define		_H_GRCPARTICLEMANAGER_

#define		PART_SYS_MAX	2
#define		PART_EMITR_MAX	100
#define		PART_PART_MAX	1000

#include	"grSingleton.h"
#include	"grParticleEmitter.h"

class		grCParticleSystem;
struct		grSParticle;
struct		grSParticleAttribute;


// TODO: Figure out how to handle draw, if it's possible to batch or whatever the solution is with SFML

class grCParticleManager : public grSingleton<grCParticleManager>
{
public:

	friend class grCore;	// Calls this update

	grCParticleManager( void );
	~grCParticleManager( void );

	grCParticleEmitter& Create( void ); // The only interaction the user should have with the manager normally

private:

	void Update( const float deltaT );

	// TODO: Fox the order for array decleration order vs. system call order. Also some array's could probably be put in a struct.

	// The system should perhaps own some of the arrays because of practicality
	uP<uP<grCParticleEmitter>[]>	m_aREmitr;
	uP<uP<grSParticleAttribute>[]>	m_aRAtt;
	grSParticle**					m_aR2DPart;	// Can't figure out how to do this with unique ptr's so...

	// Stores id's sorted from min to max so we always update linearly.
	// The goal is to read and jump in memory as efficiently as possible as the arrays are passed to different system calls.
	// Their decleration order needs to be looked over if system calls change and/or get's added, so that's a pretty big downside right now.

	vE<uInt>					m_vEActvEmitr;	// Emitter id('s) to update
	vE<uInt>					m_vEActvQue;	// Emitter id('s) that want's a particle activated
	vE<std::pair<uInt, uInt>>	m_vEDeactvQue;	// Emitter id('s) that want's a particle deactivated

	uP<grCParticleSystem>	m_uPSys;

	uInt	m_CreateCount;

	const uInt	m_cPartPerEmitr;
};

#endif	// _H_GRCPARTICLEMANAGER_
