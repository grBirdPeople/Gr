#include	"grParticleSystem.h"

#include	"grParticleEmitter.h"
#include	"grSPartAttribute.h"
#include	"grParticle.h"
#include	"grMath.h"
#include	"grRandom.h"


grCParticleSystem::grCParticleSystem( const uInt id, const sizeT partMax )
	: m_uPRand	( new grRandom() )
	, m_Id		( id )
	, m_PartMax	( partMax )
{}


grCParticleSystem::~grCParticleSystem( void )
{
	if ( m_uPRand != nullptr )
		m_uPRand.reset();
}


void
grCParticleSystem::CpyEmitrAttData( vE<uInt>& rVeActvEmitr,
									uP<uP<grCParticleEmitter>[]>& rArEmitr,
									uP<uP<grSParticleAttribute>[]>& rArAtt )
{
	if ( rVeActvEmitr.size() > 0 )
	{
		vE<sizeT> usrMods;
		usrMods.reserve( ( sizeT )grCParticleEmitter::EUsrMods::SIZE );
		sizeT actvEmitrSize = rVeActvEmitr.size();
		for ( sizeT i = 0; i < actvEmitrSize; ++i )
		{
			if ( rArEmitr[ i ]->m_UsrMods.any() == true )
			{
				uInt id = rVeActvEmitr[ i ];
				grSParticleAttribute& rEmitrAtt = *rArEmitr[ id ]->m_uPAtt.get();
				grSParticleAttribute& rSysAtt = *rArAtt[ id ].get();

				usrMods = rArEmitr[ id ]->GetUsrModsSortd();
				for ( sizeT i = 0; i < usrMods.size(); ++i )
				{
					switch ( usrMods[ i ] )
					{
						case ( sizeT )grCParticleEmitter::EUsrMods::POS:
						rSysAtt.Position = rEmitrAtt.Position;
						rSysAtt.PosOffsetRadius = rEmitrAtt.PosOffsetRadius;
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::DIR_DEG:
						rSysAtt.MinMaxDirInDeg = rEmitrAtt.MinMaxDirInDeg;
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::SPD:
						rSysAtt.MinMaxSpeed = rEmitrAtt.MinMaxSpeed;
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::LIFE_SEC:
						rSysAtt.MinMaxLife = rEmitrAtt.MinMaxLife;
						break;

						default:
						break;
					}
				}
			}
		}
	}
}


void
grCParticleSystem::UpdateSpawnClocks( uP<uP<grCParticleEmitter>[]>& rArEmitr,
									  vE<uInt>& rVeActiveEmitr,
									  vE<uInt>& rVeActivateQue,
									  const float deltaT )
{
	if ( rVeActiveEmitr.size() > 0 )
	{
		sizeT actvEmitrSize = rVeActiveEmitr.size();
		for ( sizeT i = 0; i < actvEmitrSize; ++i )
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
	}
}


void
grCParticleSystem::Activate( vE<uInt>& rVeActivateQue,
							 uP<uP<grCParticleEmitter>[]>& rArEmitr,
							 uP<uP<grSParticleAttribute>[]>& rArAtt,
							 grSParticle** pAr2DPart )
{
	if ( rVeActivateQue.size() > 0 )
	{
		sizeT queSize = rVeActivateQue.size();
		for ( sizeT i = 0; i < queSize; ++i )
		{
			grCParticleEmitter& rEmitr = *rArEmitr[ rVeActivateQue[ i ] ].get();
			grSParticleAttribute& rAtt = *rArAtt[ rEmitr.m_Id ].get();
			grSParticle& rPart = pAr2DPart[ rEmitr.m_Id ][ rEmitr.m_PartActive ];
			
			//rPart.Position = rAtt.Position;
			//rPart.Velocity = rAtt.Velocity;
			//rPart.Lifetime = rAtt.Lifetime;

			ActivatePosition( rAtt, rPart );
			ActivateVelocity( rAtt, rPart );			
			ActivateLife( rAtt, rPart );


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
	sizeT actvEmitrSize = rVeActiveEmitr.size();
	for ( sizeT emitrIdx = 0; emitrIdx < actvEmitrSize; ++emitrIdx )
	{
		sizeT id = rVeActiveEmitr[ emitrIdx ];
		sizeT partSize = rArEmitr[ id ]->m_PartActive;
		for ( sizeT i = 0; i < partSize; ++i )
		{
			grSParticle& rPart = pAr2DPart[ id ][ i ];
			
			rPart.Position += rPart.Velocity;
			//rPart.Position += rPart.Velocity * 50.0f * deltaT;

			rPart.Lifetime -= deltaT;
			if( rPart.Lifetime < 0.0f )
				rVeDeactivateQue.push_back( std::pair<uInt, uInt>( id, i ) );
		}
	}
}


void grCParticleSystem::Deactivate( vE<std::pair<uInt, uInt>>& rVeDeactivateQue,
									uP<uP<grCParticleEmitter>[]>& rArEmitr,
									grSParticle** pAr2DPart )
{
	if ( rVeDeactivateQue.size() > 0 )
	{
		sizeT queSize = rVeDeactivateQue.size();
		for ( sizeT i = 0; i < queSize; ++i )
		{
			uInt partIdx = rVeDeactivateQue[ i ].second;
			uInt id = rVeDeactivateQue[ i ].first;
			sizeT active = rArEmitr[ id ]->m_PartActive;

			--rArEmitr[ id ]->m_PartActive;

			grSParticle& rTooPart = pAr2DPart[ id ][ partIdx ];
			grSParticle& rFromPart = pAr2DPart[ id ][ active - 1 ];
			rTooPart = rFromPart;
		}

		rVeDeactivateQue.clear();
	}
}


void
grCParticleSystem::ActivatePosition( grSParticleAttribute& rAtt, grSParticle& rPart )
{
	rPart.Position = rAtt.Position;
	if ( grMath::CmpFloat( rAtt.PosOffsetRadius, 0.0f ) == false )
	{
		grV2f dir = grV2f( m_uPRand->Float( -1.0f, 1.0f ), m_uPRand->Float( -1.0f, 1.0f ) );
		float dist = m_uPRand->Float( rAtt.PosOffsetRadius, 0.0f );
		float rad = m_uPRand->Float( 0.0f, 359.9f ) * grMath::DegToRad;

		grMath::RotatePoint( &dir, rad );
		rPart.Position += dir * dist;
	}
}


void
grCParticleSystem::ActivateVelocity( grSParticleAttribute& rAtt, grSParticle& rPart )
{
	rAtt.MinMaxDirInDeg.x = grMath::Clamp( rAtt.MinMaxDirInDeg.x, 0.0f, 359.9f );
	rAtt.MinMaxDirInDeg.y = grMath::Clamp( rAtt.MinMaxDirInDeg.y, 0.0f, 359.9f );
	float deg = 0.0f;
	if ( rAtt.MinMaxDirInDeg.x > rAtt.MinMaxDirInDeg.y ||
		 grMath::CmpFloat( rAtt.MinMaxDirInDeg.x, rAtt.MinMaxDirInDeg.y ) == true )
	{
		deg = rAtt.MinMaxSpeed.x;
	}
	else if ( grMath::CmpFloat( rAtt.MinMaxDirInDeg.x, rAtt.MinMaxDirInDeg.y ) == false )
	{
		float randNum = m_uPRand->Float( rAtt.MinMaxDirInDeg.x, rAtt.MinMaxDirInDeg.y );
		deg = randNum;
	}
	grV2f dir = grV2f( 0.0f, -1.0f );	
	grMath::RotatePoint( &dir, deg * grMath::DegToRad );


	float speed = 0.0f;
	if ( rAtt.MinMaxSpeed.x > rAtt.MinMaxSpeed.y ||
		 grMath::CmpFloat( rAtt.MinMaxSpeed.x, rAtt.MinMaxSpeed.y ) == true )
	{
		speed = rAtt.MinMaxSpeed.x;
	}
	else if ( grMath::CmpFloat( rAtt.MinMaxSpeed.x, rAtt.MinMaxSpeed.y ) == false )
	{
		float randNum = m_uPRand->Float( rAtt.MinMaxSpeed.x, rAtt.MinMaxSpeed.y );
		speed = randNum;
	}

	rPart.Velocity = dir * speed;
}


void
grCParticleSystem::ActivateLife( grSParticleAttribute& rAtt, grSParticle& rPart )
{
	if ( rAtt.MinMaxLife.x > rAtt.MinMaxLife.y ||
		 grMath::CmpFloat( rAtt.MinMaxLife.x, rAtt.MinMaxLife.y ) == true )
	{
		rPart.Lifetime = rAtt.MinMaxLife.x;
		return;
	}
	if ( grMath::CmpFloat( rAtt.MinMaxLife.x, rAtt.MinMaxLife.y ) == false )
	{
		float randLife = m_uPRand->Float( rAtt.MinMaxLife.x, rAtt.MinMaxLife.y );
		rPart.Lifetime = randLife;
	}
}
