#include	"grParticleSystem.h"

#include	"grParticleEmitter.h"
#include	"grSPartAttribute.h"
#include	"grParticle.h"


grCParticleSystem::grCParticleSystem( const uInt id, const sizeT partMax )
	: m_Id		( id )
	, m_PartMax	( partMax )
{}


void
grCParticleSystem::UpdateSpawnClocks( uP<uP<grCParticleEmitter>[]>& rArEmitr,
									  vE<uInt>& rVeActiveEmitr,
									  vE<uInt>& rVeActivateQue,
									  const float deltaT )
{
	if ( rVeActiveEmitr.size() > 0 )
	{
		for ( sizeT i = 0; i < rVeActiveEmitr.size(); ++i )
		{
			grCParticleEmitter& rEmitr = *rArEmitr[ rVeActiveEmitr[ i ] ].get();
			rEmitr.m_SpawnTimer -= deltaT;
			if ( rEmitr.m_SpawnTimer < 0.0f )
			{
				rEmitr.m_SpawnTimer += rEmitr.m_SpawnRate;
				if( rEmitr.m_PartActive < m_PartMax )
					rVeActivateQue.push_back( rEmitr.m_Id );
			}
		}

		std::sort( rVeActivateQue.begin(), rVeActivateQue.end() );
	}
}


void
grCParticleSystem::CpyEmitrAttData( vE<uInt>& rVeUsrQue,
									uP<uP<grCParticleEmitter>[]>& rArEmitr,
									uP<uP<grSParticleAttribute>[]>& rArAtt )
{
	if ( rVeUsrQue.size() > 0 )
	{
		for ( sizeT i = 0; i < rVeUsrQue.size(); ++i )
		{
			grCParticleEmitter& rEmitr = *rArEmitr[ rVeUsrQue[ i ] ].get();
			grSParticleAttribute& rAtt = *rArAtt[ rVeUsrQue[ i ] ].get();

			// TODO: Not used atm because it needs to be built

			// Would be nice to have exactly which data has changed, saved in a bitset either in the emitr or the att, or something
		}

		rVeUsrQue.clear();
	}
}


void
grCParticleSystem::Activate( vE<uInt>& rVeActivateQue,
							 uP<uP<grCParticleEmitter>[]>& rArEmitr,
							 uP<uP<grSParticleAttribute>[]>& rArAtt,
							 grSParticle** pArPart )
{
	if ( rVeActivateQue.size() > 0 )
	{
		for ( sizeT i = 0; i < rVeActivateQue.size(); ++i )
		{
			grCParticleEmitter& rEmitr = *rArEmitr[ rVeActivateQue[ i ] ].get();
			grSParticleAttribute& rAtt = *rArAtt[ rEmitr.m_Id ].get();
			grSParticle& rPart = pArPart[ rEmitr.m_Id ][ rEmitr.m_PartActive ];

			rPart.Position = rAtt.Position;
			rPart.Velocity = rAtt.Velocity;
			rPart.Lifetime = rAtt.Lifetime;

			++rEmitr.m_PartActive;
		}

		rVeActivateQue.clear();
	}
}


void
grCParticleSystem::Update( vE<std::pair<uInt, uInt>>& rVeDeactivateQue,
						   vE<uInt>& rVeActiveEmitr,
						   uP<uP<grCParticleEmitter>[]>& rArEmitr,
						   uP<uP<grSParticleAttribute>[]>& rArAtt,
						   grSParticle** pAr2DPart,
						   const float deltaT )
{
	for ( sizeT emitrIdx = 0; emitrIdx < rVeActiveEmitr.size(); ++emitrIdx )
	{
		sizeT id = rVeActiveEmitr[ emitrIdx ];
		sizeT size = rArEmitr[ id ]->m_PartActive;
		//grSParticleAttribute& rAtt = *rArAtt[ id ].get();
		for ( sizeT part = 0; part < size; ++part )
		{
			grSParticle& rPart = pAr2DPart[ id ][ part ];

			// rPart.Velocity... something something
			rPart.Position += rPart.Velocity * 50.0f * deltaT;

			rPart.Lifetime -= deltaT;
			if( rPart.Lifetime < 0.0f )
				rVeDeactivateQue.push_back( std::pair<uInt, uInt>( id, part ) );
		}
	}
}


void grCParticleSystem::Deactivate( vE<std::pair<uInt, uInt>>& rVeDeactivateQue, uP<uP<grCParticleEmitter>[]>& rArEmitr, grSParticle** pAr2DPart )
{
	if ( rVeDeactivateQue.size() > 0 )
	{
		for ( sizeT i = 0; i < rVeDeactivateQue.size(); ++i )
		{
			uInt id = rVeDeactivateQue[ i ].first;
			uInt partIdx = rVeDeactivateQue[ i ].second;
			sizeT active = rArEmitr[ id ]->m_PartActive;

			grSParticle& rTooPart = pAr2DPart[ id ][ partIdx ];
			grSParticle& rFromPart = pAr2DPart[ id ][ active - 1 ];

			rTooPart = rFromPart;   // TODO: Make sure this actually copies the values

			// Should not need this but in case
			//rFromPart.Position = 0.0f;
			//rFromPart.Velocity = 0.0f;
			//rFromPart.Lifetime = 0.0f;

			--rArEmitr[ id ]->m_PartActive;
		}

		rVeDeactivateQue.clear();
	}
}
