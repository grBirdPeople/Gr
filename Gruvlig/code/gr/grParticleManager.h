#ifndef		_H_GRPARTICLEMANAGER_
#define		_H_GRPARTICLEMANAGER_

#define		MAX_PARTCILE_SYS	50

#include	"grSingleton.h"

class		grParticleSystem;


// grParticleManager
//////////////////////////////////////////////////
class grParticleManager : public grSingleton<grParticleManager>
{
public:

								grParticleManager		( void );
								~grParticleManager		( void );

	//////////////////////////////////////////////////

	grParticleSystem*	const	CreateParticleSystem	( void );
	void						Update					( const float deltaT );

	//////////////////////////////////////////////////

private:

	grParticleSystem*	m_arrParticleSys[ MAX_PARTCILE_SYS ];

	uInt				m_SystemQuantity;

};

#endif	// _H_GRPARTICLEMANAGER_
