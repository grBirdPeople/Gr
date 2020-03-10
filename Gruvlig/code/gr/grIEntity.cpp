#include	"grIEntity.h"

#include	"grDefine.h"
#include	"grEntityManager.h"


// cTor
//////////////////////////////////////////////////
grIEntity::grIEntity( void )
	: m_Name		( "No Name" )
	, m_Id			( 0 )
	, m_Type		( EEntityType::NONE )
	, m_bIsEnabled	( false )
	, rEMan			( grEntityManager::Instance() )
	, m_pParent		( nullptr )
	, m_PosWorld	( grV2f() )
	, m_PosLocal	( grV2f() )
{}


// dTor
//////////////////////////////////////////////////
grIEntity::~grIEntity( void )
{
	if ( m_pParent != nullptr )
	{
		m_pParent->ReleaseChildById( m_Id );
	}

	if ( m_vecChildren.size() > 0 )
	{
		DestroyRecursively( *this );
	}
}


// GetChildById
//////////////////////////////////////////////////
grIEntity*
grIEntity::GetChildById( const uInt id )
{
	for ( uInt i = 0; i < m_vecChildren.size(); ++i )
	{
		if ( m_vecChildren[ i ]->GetId() == id )
		{
			return m_vecChildren[ i ];
		}
	}

	return nullptr;
}


// GetChild
//////////////////////////////////////////////////
grIEntity*
grIEntity::GetChildByIdx( const uInt idx )
{
	if ( m_vecChildren.size() > 0 && m_vecChildren.size() > idx )
	{
		return m_vecChildren[ idx ];
	}

	return nullptr;
}


// SetIsActive
//////////////////////////////////////////////////
void
grIEntity::SetEnable( const bool bEnable )
{
	if ( bEnable == m_bIsEnabled )
	{
		return;
	}

	m_bIsEnabled = bEnable;
	( bEnable == true ) ? rEMan.Enable( this ) : rEMan.Disable( this );
	EnableDisableRecursively( *this );
}


// AddChild
//////////////////////////////////////////////////
void
grIEntity::AddChild( grIEntity* pChild )
{
	// TODO: Somehow check if child is already added

	grV2f childPos = pChild->GetPosition();
	grV2f parentPos = m_PosWorld;
	grV2f offsetPos;

	offsetPos.x = childPos.x - parentPos.x;
	offsetPos.y = childPos.y - parentPos.y;

	pChild->SetPosition( offsetPos );
	pChild->SetParent( this );
	m_vecChildren.push_back( pChild );
}


// ReleaseChild by id
//////////////////////////////////////////////////
void
grIEntity::ReleaseChildById( const uInt id )
{
	// TODO: When there is hash, smoke some here

	for ( uInt i = 0; i < m_vecChildren.size(); ++i )
	{
		if ( m_vecChildren[ i ]->GetId() == ( uInt )id )
		{
			m_vecChildren[ i ]->ResetPosOnRelease();
			m_vecChildren[ i ]->SetParent( nullptr );
			m_vecChildren.erase( m_vecChildren.begin() + i );
			return;
		}
	}

#ifdef DEBUG
	std::cerr << "grIEntity::ReleaseChildById(): Entity has no child with id: " << id << '\n' << std::endl;
#endif // DEBUG
}


// ReleaseChild by idx
//////////////////////////////////////////////////
void
grIEntity::ReleaseChildByIdx( const uInt idx )
{
#ifdef DEBUG
	sInt size = ( sInt )m_vecChildren.size() - 1;
	if ( idx > size )
	{
		std::cerr << "grIEntity::ReleaseChildByIdx(): Out of range. Idx was: " << idx << std::endl;
		return;
	}

	if ( size < 0 )
	{
		std::cerr << "grIEntity::ReleaseChildByIdx(): Entity has no children" << std::endl;
		return;
	}
#endif // DEBUG

	m_vecChildren[ idx ]->ResetPosOnRelease();
	m_vecChildren[ idx ]->SetParent( nullptr );
	m_vecChildren.erase( m_vecChildren.begin() + idx );
}


// Update
//////////////////////////////////////////////////
void
grIEntity::Update( void )
{
	// TODO: Add scale and rotation and whatever

	if ( m_pParent == nullptr )
	{
		m_PosWorld = m_PosLocal;

		if ( m_vecChildren.size() > 0 )
		{
			UpdatePosRecursively( *this );
		}
	}
}


// EnableDisableRecursively
//////////////////////////////////////////////////
void
grIEntity::EnableDisableRecursively( grIEntity& rEntity )
{
	if ( rEntity.m_vecChildren.size() > 0 )
	{
		for ( sInt i = ( sInt )rEntity.m_vecChildren.size() - 1; i >= 0; --i )	// TODO: Fix loop as this only works because of casting
		{
			grIEntity& ent = *rEntity.m_vecChildren[ i ];
			if ( ent.m_vecChildren.size() > 0 )
			{
				EnableDisableRecursively( ent );
			}

			ent.m_bIsEnabled = m_bIsEnabled;
			( m_bIsEnabled == true ) ? rEMan.Enable( &ent ) : rEMan.Disable( &ent );
		}
	}
}


// UpdatePosRecursively
//////////////////////////////////////////////////
void
grIEntity::UpdatePosRecursively( grIEntity& rEntity )
{
	if ( rEntity.m_vecChildren.size() > 0 )
	{
		for ( uInt i = 0; i < rEntity.m_vecChildren.size(); ++i )
		{
			grIEntity& ent = *rEntity.m_vecChildren[ i ];
			grV2f parentPos = ent.m_pParent->GetPosition();
			ent.m_PosWorld = parentPos + ent.m_PosLocal;

			if ( ent.m_vecChildren.size() > 0 )
			{
				UpdatePosRecursively( ent );
			}
		}
	}
}


// DestroyRecursively
//////////////////////////////////////////////////
void
grIEntity::DestroyRecursively( grIEntity& rEntity )
{
	if ( rEntity.m_vecChildren.size() > 0 )
	{
		for ( uInt i = 0; i < rEntity.m_vecChildren.size(); ++i )
		{
			DestroyRecursively( *rEntity.m_vecChildren[ i ] );
		}
	}

	grIEntity* pParent = rEntity.m_pParent;
	if ( pParent == nullptr )
	{
		return;
	}

	pParent->ReleaseChildById( rEntity.m_Id );
	grEntityManager::Instance().DestroyEntity( &rEntity );
}


// ResetPosOnRelease
//////////////////////////////////////////////////
void
grIEntity::ResetPosOnRelease( void )
{
	m_PosLocal = m_pParent->m_PosWorld + m_PosLocal;
}
