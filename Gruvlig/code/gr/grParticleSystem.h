#ifndef		_H_GRPARTICLE_SYSTEM_
#define		_H_GRPARTICLE_SYSTEM_

#include	"grCommon.h"

struct		grSParticleAttribute;
class		grCParticleEmitter;
struct		grSParticle;
class		grRandom;


class grCParticleSystem
{
public:

	grCParticleSystem( const uInt id, const sizeT partMax );
	~grCParticleSystem( void );

	grCParticleSystem( const grCParticleSystem& ) = delete;
	grCParticleSystem& operator=( const grCParticleSystem& ) = delete;

	void Copy( vE<uInt>& rVeActiveEmitr,
			   uP<uP<grCParticleEmitter>[]>& rArEmitr,
			   uP<uP<grSParticleAttribute>[]>& rArAtt );

	// TODO: Perhaps merge this with activate, uses the same stuff anyways and the code in spawn is basic
	void Spawn( uP<uP<grCParticleEmitter>[]>& rArEmitr,
				vE<uInt>& rVeActiveEmitr,
				vE<uInt>& rVeActivateQue,
				const float deltaT );

	void Activate( vE<uInt>& rVeActivateQue,
				   uP<uP<grCParticleEmitter>[]>& rArEmitr,
				   uP<uP<grSParticleAttribute>[]>& rArAtt,
				   grSParticle** pArPart );

	void Update( vE<std::pair<uInt, uInt>>& rVeDeactivateQue,
				 vE<uInt>& rVeActiveEmitr,
				 uP<uP<grCParticleEmitter>[]>& rArEmitr,
				 uP<uP<grSParticleAttribute>[]>& rArAtt,
				 grSParticle** pAr2DPart,
				 const float deltaT );

	void Deactivate( vE<std::pair<uInt, uInt>>& rVeDeactivateQue,
					 uP<uP<grCParticleEmitter>[]>& rArEmitr,
					 grSParticle** pAr2DPart );

private:

	void ActvPosition( grSParticleAttribute& rAtt, grSParticle& rPart );
	void ActvVelocity( grSParticleAttribute& rAtt, grSParticle& rPart );
	void ActvLife( grSParticleAttribute& rAtt, grSParticle& rPart );

	uP<grRandom>	m_uPRand;

	uInt	m_Id,
			m_PartMax; // A little weird // Defined in part man and passed to this from part man create
};

#endif	// _H_GRPARTICLE_SYSTEM_
