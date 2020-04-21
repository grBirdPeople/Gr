#include	"grCParticleManager.h"

#include	"grParticleSystem.h"
#include	"grSPartAttribute.h"
#include	"grParticle.h"
#include	"grAlgo.h"

// TEST DRAW
#include	"grBBox.h"
#include	"grDebugManager.h"
// TEST DRAW


grCParticleManager::grCParticleManager( void )
	: m_aREmitr			( new uP<grCParticleEmitter>[ PART_EMITR_MAX ] )
	, m_aRAtt			( new uP<grSParticleAttribute>[ PART_EMITR_MAX ] )
	, m_aR2DPart		( new grSParticle*[ PART_EMITR_MAX ] )

	, m_uPSys			( new grCParticleSystem( 0, PART_PART_MAX ) )

	, m_CreateCount		( 0 )
	, m_cPartPerEmitr	( PART_PART_MAX )
{
	m_vEActiveEmitr.reserve( PART_EMITR_MAX );
	m_vEActivateQue.reserve( PART_EMITR_MAX );
	m_vEDeactivateQue.reserve( PART_EMITR_MAX );
}


grCParticleManager::~grCParticleManager( void )
{
	for ( sizeT i = 0; i < PART_EMITR_MAX; ++i )
	{
		if( m_aREmitr[ i ].get() != nullptr )
			m_aREmitr[ i ].reset();
	}
	delete[] m_aREmitr.release();

	for ( sizeT i = 0; i < PART_EMITR_MAX; ++i )
	{
		if ( m_aRAtt[ i ].get() != nullptr )
			m_aRAtt[ i ].reset();
	}
	delete[] m_aRAtt.release();

	for ( sizeT i = 0; i < PART_EMITR_MAX; ++i )
	{
		if ( m_aR2DPart[ i ] != nullptr )
			delete[] m_aR2DPart[ i ];
	}
	delete[] m_aR2DPart;

	delete m_uPSys.release();
}


grCParticleEmitter&
grCParticleManager::Create( void )
{
	m_vEActiveEmitr.push_back( m_CreateCount );

	m_aREmitr[ m_CreateCount ] = std::make_unique<grCParticleEmitter>( m_CreateCount );
	m_aRAtt[ m_CreateCount ] = std::make_unique<grSParticleAttribute>();
	m_aR2DPart[ m_CreateCount ] = new grSParticle[ m_cPartPerEmitr ];

	++m_CreateCount;

	return *m_aREmitr[ m_CreateCount - 1 ].get();
}


void
grCParticleManager::Update( const float deltaT )
{
	m_uPSys->UpdateSpawnClocks( m_aREmitr, m_vEActiveEmitr, m_vEActivateQue, deltaT );
	m_uPSys->CpyEmitrAttData( m_vEActiveEmitr, m_aREmitr, m_aRAtt );
	m_uPSys->Activate( m_vEActivateQue, m_aREmitr, m_aRAtt, m_aR2DPart );
	m_uPSys->Update( m_vEDeactivateQue, m_vEActiveEmitr, m_aREmitr, m_aRAtt, m_aR2DPart, deltaT );
	m_uPSys->Deactivate( m_vEDeactivateQue, m_aREmitr, m_aR2DPart );

	// TEST DRAW
	for ( sizeT id = 0; id < m_vEActiveEmitr.size(); ++id )
	{
		uInt active = m_aREmitr[ id ]->m_PartActive;
		for ( sizeT part = 0; part < active; ++part )
		{
			grBBox box( grV2f( 20.0f, 20.0f ), m_aR2DPart[ m_vEActiveEmitr[ id ] ][ part ].Position );
			grDebugManager::Instance().AddBBox( box, sf::Color( 255, 125, 0 ) );
		}
	}
	// TEST DRAW
}
