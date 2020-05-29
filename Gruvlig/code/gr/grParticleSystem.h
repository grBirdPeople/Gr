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

	grCParticleSystem( const intU id, const sizeT partMax );
	~grCParticleSystem( void );

	grCParticleSystem( const grCParticleSystem& ) = delete;
	grCParticleSystem& operator=( const grCParticleSystem& ) = delete;

	void Copy( vE<intU>& rVeActiveEmitr,
			   pU<pU<grCParticleEmitter>[]>& rArEmitr,
			   pU<pU<grSParticleAttribute>[]>& rArAtt );

	// TODO: Perhaps merge this with activate, uses the same stuff anyways and the code in spawn is basic
	void QueSpawn( pU<pU<grCParticleEmitter>[]>& rArEmitr,
				vE<intU>& rVeActiveEmitr,
				vE<intU>& rVeActivateQue,
				const float deltaT );

	void Activate( vE<intU>& rVeActivateQue,
				   pU<pU<grCParticleEmitter>[]>& rArEmitr,
				   pU<pU<grSParticleAttribute>[]>& rArAtt,
				   grSParticle** pArPart );

	void Update( vE<std::pair<intU, intU>>& rVeDeactivateQue,
				 vE<intU>& rVeActiveEmitr,
				 pU<pU<grCParticleEmitter>[]>& rArEmitr,
				 pU<pU<grSParticleAttribute>[]>& rArAtt,
				 grSParticle** pAr2DPart,
				 const float deltaT );

	void Deactivate( vE<std::pair<intU, intU>>& rVeDeactivateQue,
					 pU<pU<grCParticleEmitter>[]>& rArEmitr,
					 grSParticle** pAr2DPart );

private:

	void ActvPosition( grSParticleAttribute& rAtt, grSParticle& rPart );
	void ActvVelocity( grSParticleAttribute& rAtt, grSParticle& rPart );
	void ActvLife( grSParticleAttribute& rAtt, grSParticle& rPart );
	void ActvColor( grSParticleAttribute& rAtt, grSParticle& rPart );	// Change over T is dependent of lifetime so allways do color after

	pU<grRandom>	m_uPRand;

	intU	m_Id,
			m_PartMax; // A little weird // Defined in part man and passed to this from part man create
};

#endif	// _H_GRPARTICLE_SYSTEM_
