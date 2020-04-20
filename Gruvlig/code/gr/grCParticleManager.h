#ifndef		_H_GRCPARTICLEMANAGER_
#define		_H_GRCPARTICLEMANAGER_

#define		PART_SYS_MAX	2
#define		PART_EMITR_MAX	5
#define		PART_PART_MAX	100

#include	"grSingleton.h"
#include	"grParticleEmitter.h"

class		grCParticleSystem;
struct		grSParticle;
struct		grSParticleAttribute;


class grCParticleManager : public grSingleton<grCParticleManager>
{
public:

	friend class grCore;			// Calls this update

	grCParticleManager( void );
	~grCParticleManager( void );

	grCParticleEmitter& Create( void );

private:

	void Update( const float deltaT );

	void AddParticleSpawn( const uInt id );

	// The system should probably own some of the arrays because of practicality
	uP<uP<grCParticleEmitter>[]>	m_aREmitr;
	uP<uP<grSParticleAttribute>[]>	m_aRAtt;
	grSParticle**					m_aR2DPart;	// Can't figure out how to do this with unique ptr's so

	uP<uP<grSParticleAttribute>[]> hej;

	// Stores id's sorted min/max so we can update things linearly
	vE<uInt>					m_vEActiveEmitr;	// Emitter(s) to update
	vE<uInt>					m_vEActivateQue;	// Emitter(s) that want's a particle activated
	vE<std::pair<uInt, uInt>>	m_vEDeactivateQue;	// Emitter(s) that want's a particle deactivated
	vE<uInt>					m_vEUsrQue;			// Emitter(s) the user has done changes to

	uP<grCParticleSystem>	m_uPSys;

	uInt	m_CreateCount;

	const uInt	m_cPartPerEmitr;	// TODO: Could have a few predefined options for the user to choose from
};


#endif	// _H_GRCPARTICLEMANAGER_
