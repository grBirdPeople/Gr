#include	"grIEntity.h"

#include	"grEntityManager.h"


// cTor
//////////////////////////////////////////////////
grIEntity::grIEntity( void )
	: m_Name		( "No Name" )
	, m_Id			( 0 )
	, m_Type		( grEnums::EntityType::NONE )
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
inline grIEntity*
grIEntity::GetChildById( const intU id )
{
	for ( intU i = 0; i < m_vecChildren.size(); ++i )
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
grIEntity::GetChildByIdx( const intU idx )
{
	if ( m_vecChildren.size() > 0 && m_vecChildren.size() > idx )
	{
		return m_vecChildren[ idx ];
	}

	return nullptr;
}


// SetIsActive
//////////////////////////////////////////////////
inline void
grIEntity::SetEnable( const bool bEnable )
{
	if ( bEnable == m_bIsEnabled )
	{ return; }

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
grIEntity::ReleaseChildById( const intU id )
{
	// TODO: When there is hash, smoke some here

	for ( intU i = 0; i < m_vecChildren.size(); ++i )
	{
		if ( m_vecChildren[ i ]->GetId() == ( intU )id )
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
grIEntity::ReleaseChildByIdx( const intU idx )
{
#ifdef DEBUG
	intS size = ( intS )m_vecChildren.size() - 1;
	if ( ( intS )idx > size )
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
		for ( intS i = ( intS )rEntity.m_vecChildren.size() - 1; i >= 0; --i )	// TODO: Fix loop as this only works because of casting which also is stupid overhead anyway
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
		for ( intU i = 0; i < rEntity.m_vecChildren.size(); ++i )
		{
			grIEntity& rEnt = *rEntity.m_vecChildren[ i ];
			grV2f parentPos = rEnt.m_pParent->GetPosition();
			rEnt.m_PosWorld = parentPos + rEnt.m_PosLocal;

			if ( rEnt.m_vecChildren.size() > 0 )
			{
				UpdatePosRecursively( rEnt );
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
		for ( intU i = 0; i < rEntity.m_vecChildren.size(); ++i )
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
inline void
grIEntity::ResetPosOnRelease( void )
{
	m_PosLocal = m_pParent->m_PosWorld + m_PosLocal;
}
