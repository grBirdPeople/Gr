#ifndef		_H_GRPARTICLE_SYSTEM_
#define		_H_GRPARTICLE_SYSTEM_

#include	"grCommon.h"

struct		grSParticleAttribute;
class		grCParticleEmitter;
struct		grSParticle;


class grCParticleSystem
{
public:

	grCParticleSystem( const uInt id, const sizeT partMax );

	grCParticleSystem( const grCParticleSystem& ) = delete;
	grCParticleSystem& operator=( const grCParticleSystem& ) = delete;

	void UpdateSpawnClocks( uP<uP<grCParticleEmitter>[]>& rArEmitr,
							vE<uInt>& rVeActiveEmitr,
							vE<uInt>& rVeActivateQue,
							const float deltaT );

	void CpyEmitrAttData( vE<uInt>& rVeActiveEmitr,
						  uP<uP<grCParticleEmitter>[]>& rArEmitr,
						  uP<uP<grSParticleAttribute>[]>& rArAtt );

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

	uInt	m_Id,
			m_PartMax; // Don't really like this
};

#endif	// _H_GRPARTICLE_SYSTEM_
