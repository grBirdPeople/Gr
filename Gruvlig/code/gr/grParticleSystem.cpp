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
grCParticleSystem::Copy( vE<uInt>& rVeActvEmitr,
						 uP<uP<grCParticleEmitter>[]>& rArEmitr,
						 uP<uP<grSParticleAttribute>[]>& rArAtt )
{
	if ( rVeActvEmitr.size() > 0 )
	{
		sizeT actvEmitrSize = rVeActvEmitr.size();
		for ( sizeT i = 0; i < actvEmitrSize; ++i )
		{
			if ( rArEmitr[ i ]->m_UsrMods.any() == true )
			{
				vE<sizeT> usrMods;
				usrMods.reserve( ( sizeT )grCParticleEmitter::EUsrMods::SIZE );
				float x = 0.0f, y = 0.0f;
				uInt id = rVeActvEmitr[ i ];
				grSParticleAttribute& rEmitrAtt = *rArEmitr[ id ]->m_uPAtt.get();
				grSParticleAttribute& rSysAtt = *rArAtt[ id ].get();
				usrMods = rArEmitr[ id ]->GetUsrModsSortd();
				for ( sizeT i = 0; i < usrMods.size(); ++i )
				{
					switch ( usrMods[ i ] )
					{
						case ( sizeT )grCParticleEmitter::EUsrMods::POS:
						rSysAtt.EmitrPos = rEmitrAtt.EmitrPos;
						rSysAtt.PartPosOffsetRadius = rEmitrAtt.PartPosOffsetRadius;
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::DIR_EMITR:
						rSysAtt.EmitrRotInDeg = grMath::Clamp( rEmitrAtt.EmitrRotInDeg, 0.0f, 359.9f );
						grMath::RotatePoint( &rSysAtt.EmitrDir, rSysAtt.EmitrRotInDeg * grMath::DegToRad );
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::DIR_PART:
						rSysAtt.PartMinMaxRotInDeg.x = grMath::Clamp( rEmitrAtt.PartMinMaxRotInDeg.x, 0.0f, 359.9f );
						rSysAtt.PartMinMaxRotInDeg.y = grMath::Clamp( rEmitrAtt.PartMinMaxRotInDeg.y, 0.0f, 359.9f );
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::SPD:
						rSysAtt.PartMinMaxSpd = grMath::Clamp( rEmitrAtt.PartMinMaxSpd.x, rEmitrAtt.PartMinMaxSpd.x, rEmitrAtt.PartMinMaxSpd.y );
						rSysAtt.PartModSpd = rEmitrAtt.PartModSpd;
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::LIFE:
						x = grMath::Clamp( rEmitrAtt.PartMinMaxLife.x, rEmitrAtt.PartMinMaxLife.x, rEmitrAtt.PartMinMaxLife.y );
						y = grMath::Clamp( rEmitrAtt.PartMinMaxLife.y, rEmitrAtt.PartMinMaxLife.x, rEmitrAtt.PartMinMaxLife.y );
						rSysAtt.PartMinMaxLife.x = grMath::Clamp( rEmitrAtt.PartMinMaxLife.x, 0.0f, x );
						rSysAtt.PartMinMaxLife.y = grMath::Clamp( rEmitrAtt.PartMinMaxLife.y, 0.0f, y );
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
grCParticleSystem::Spawn( uP<uP<grCParticleEmitter>[]>& rArEmitr,
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

			ActvPosition( rAtt, rPart );
			ActvVelocity( rAtt, rPart );
			ActvLife( rAtt, rPart );

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
		sizeT emitrId = rVeActiveEmitr[ emitrIdx ];
		sizeT partSize = rArEmitr[ emitrId ]->m_PartActive;
		for ( sizeT partIdx = 0; partIdx < partSize; ++partIdx )
		{
			grSParticle& rPart = pAr2DPart[ emitrId ][ partIdx ];

			rPart.Spd -= rPart.SpdMod;
			grV2f SpdAccel = rPart.Direction * rPart.Spd;

			grV2f grav = grV2f( 0.0f, 75.0f );
			
			rPart.Position += ( SpdAccel ) * deltaT;

			rPart.Lifetime -= deltaT;
			if ( rPart.Lifetime < 0.0f )
				rVeDeactivateQue.push_back( std::pair<uInt, uInt>( emitrId, partIdx ) );
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
		for ( sizeT idx = 0; idx < queSize; ++idx )
		{
			uInt emitrId = rVeDeactivateQue[ idx ].first;
			uInt partIdx = rVeDeactivateQue[ idx ].second;
			sizeT partsActive = rArEmitr[ emitrId ]->m_PartActive;

			--rArEmitr[ emitrId ]->m_PartActive;

			grSParticle& rTooPart = pAr2DPart[ emitrId ][ partIdx ];
			grSParticle& rFromPart = pAr2DPart[ emitrId ][ partsActive - 1 ];
			rTooPart = rFromPart;
		}

		rVeDeactivateQue.clear();
	}
}


void
grCParticleSystem::ActvPosition( grSParticleAttribute& rAtt, grSParticle& rPart )
{
	rPart.Position = rAtt.EmitrPos;
	if ( grMath::CmpFloat( rAtt.PartPosOffsetRadius, 0.0f ) != true )
	{
		grV2f dir = grV2f( m_uPRand->Float( -1.0f, 1.0f ), m_uPRand->Float( -1.0f, 1.0f ) );
		float dist = m_uPRand->Float( 0.0f, rAtt.PartPosOffsetRadius );
		float rad = m_uPRand->Float( 0.0f, 359.9f ) * grMath::DegToRad;

		grMath::RotatePoint( &dir, rad );
		rPart.Position += dir * dist;
	}
}


void
grCParticleSystem::ActvVelocity( grSParticleAttribute& rAtt, grSParticle& rPart )
{
	// Direction
	float deg = 0.0f;
	if ( grMath::CmpFloat( rAtt.PartMinMaxRotInDeg.x, rAtt.PartMinMaxRotInDeg.y ) != true )
	{
		if ( rAtt.PartMinMaxRotInDeg.x > rAtt.PartMinMaxRotInDeg.y )	
		{
			float diff = ( 360.0f - rAtt.PartMinMaxRotInDeg.x );
			deg = m_uPRand->Float( 0.0f, diff + rAtt.PartMinMaxRotInDeg.y );
			deg -= diff;
			if ( 0.0f > deg )
				deg = 360.0f + deg;
		}
		else
		{
			deg = m_uPRand->Float( rAtt.PartMinMaxRotInDeg.x, rAtt.PartMinMaxRotInDeg.y );
		}
	}
	else
	{
		deg = rAtt.PartMinMaxRotInDeg.x;
	}

	deg += rAtt.EmitrRotInDeg;
	rPart.Direction = rAtt.EmitrDir;
	grMath::RotatePoint( &rPart.Direction, deg * grMath::DegToRad );

	// Speed
	if ( rAtt.PartMinMaxSpd.x > rAtt.PartMinMaxSpd.y )
		rAtt.PartMinMaxSpd.x = rAtt.PartMinMaxSpd.y;

	rAtt.PartMinMaxSpd.x = grMath::Clamp( rAtt.PartMinMaxSpd.x, rAtt.PartMinMaxSpd.x, rAtt.PartMinMaxSpd.y );

	rPart.Spd = ( grMath::CmpFloat( rAtt.PartMinMaxSpd.x, rAtt.PartMinMaxSpd.y ) != true )
		? m_uPRand->Float( rAtt.PartMinMaxSpd.x, rAtt.PartMinMaxSpd.y )
		: rAtt.PartMinMaxSpd.x;

	//float weight = m_uPRand->Float( 1.0f, 1.5f );
	//rPart.Spd /= weight;

	rPart.SpdMod = rAtt.PartModSpd;




	//grV2f dir = grV2f( 0.0f, -1.0f );	
	//grMath::RotatePoint( &dir, deg * grMath::DegToRad );

	//float speed = ( grMath::CmpFloat( rAtt.MinMaxSpeed.x, rAtt.MinMaxSpeed.y ) != true )
	//	? m_uPRand->Float( rAtt.MinMaxSpeed.x, rAtt.MinMaxSpeed.y )
	//	: rAtt.MinMaxSpeed.x;

	//rPart.Velocity = dir * speed;
}


void
grCParticleSystem::ActvLife( grSParticleAttribute& rAtt, grSParticle& rPart )
{
	rPart.Lifetime = ( grMath::CmpFloat( rAtt.PartMinMaxLife.x, rAtt.PartMinMaxLife.y ) != true )
		? m_uPRand->Float( rAtt.PartMinMaxLife.x, rAtt.PartMinMaxLife.y )
		: rAtt.PartMinMaxLife.x;
}
