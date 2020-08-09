#ifndef		_H_GRCPARTICLEMANAGER_
#define		_H_GRCPARTICLEMANAGER_

#define		PART_SYS_MAX	2
#define		PART_EMITR_MAX	100
#define		PART_PART_MAX	5000

#include	"grSingleton.h"
#include	"grParticleEmitter.h"

class		grCParticleSystem;
struct		grSParticleArr;
struct		grSParticleAttribute;


// TODO: Figure out how to handle draw, if it's possible to batch or whatever the solution is with SFML

class grCParticleManager : public grSingleton<grCParticleManager>
{
public:

	friend class grCore;	// Calls this update

	grCParticleManager( void );
	~grCParticleManager( void );

	grCParticleEmitter& Create( void ); // The only interaction the user should have with the manager under regular circumstances

private:

	void Update( const float deltaT );

	// TODO: Fox the order for array decleration order vs. system call order. Also some array's could probably be put in a struct.

	// The system should perhaps own some of the arrays because of practicality, or not because of straightforwardness... which definitely is a word
	pU<pU<grCParticleEmitter>[]>	m_aREmitr;
	pU<pU<grSParticleAttribute>[]>	m_aRAtt;
	grSParticleArr**					m_aR2DPart;	// Can't figure out how to do this with unique ptr's so...

	// Stores id's sorted from min to max so we always update linearly.
	// The goal is to read/write/jump in memory as efficiently as possible as the arrays are passed to different system calls.
	// Their decleration order needs to be looked over if system calls change and/or get's added, so that's a pretty big downside right now.

	vE<intU>			m_vEActvEmitr;	// Emitter id('s) to update
	vE<intU>			m_vEActvQue;	// Emitter id('s) that want's a particle activated
	vE<pR<intU, intU>>	m_vEDeactvQue;	// Emitter id('s) that want's a particle deactivated

	pU<grCParticleSystem>	m_uPSys;

	intU	m_CreateCount;

	const intU	m_cPartPerEmitr;
};

#endif	// _H_GRCPARTICLEMANAGER_
