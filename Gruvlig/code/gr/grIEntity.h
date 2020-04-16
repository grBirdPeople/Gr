#ifndef		_GRIENTITY_H_
#define		_GRIENTITY_H_

#include	"grCommon.h"
#include	"grDefine.h"
#include	"grV2.h"
#include	"grEnums.h"
#include	"grBBox.h"

class		grEntityManager;
class		grIEntity;

using 		vecIEntity = std::vector<grIEntity*>;


// grIEntity
//////////////////////////////////////////////////
class grIEntity
{
public:

	grIEntity( void );
	~grIEntity( void );
	
	//////////////////////////////////////////////////
	
	inline grIEntity* GetChildById( const uInt id );
	grIEntity* GetChildByIdx( const uInt idx );
	const str& GetName( void ) const { return m_Name; }
	void SetName( const str& rName ) { m_Name = rName; }
	uInt GetId( void ) const { return m_Id; }
	grEnums::EntityType	GetType( void ) const { return m_Type; }
	bool GetEnable( void ) const { return m_bIsEnabled; }
	inline void SetEnable( const bool bEnable );
	void SetPosition( const grV2f& rPos ) { m_PosLocal = rPos; }
	const grV2f& GetPosition( void ) const { return m_PosWorld; }
	void SetParent( grIEntity* pParent ) { m_pParent = pParent; }
	const vecIEntity& GetChildVec( void ) const { return m_vecChildren; }

	//////////////////////////////////////////////////

	void AddChild( grIEntity* pChild );
	void ReleaseChildById( const uInt id );
	void ReleaseChildByIdx( const uInt idx );
	void Update( void );	// Call this from every derived class Update() (technically only necessary if the instance has either a parent or children set)

	//////////////////////////////////////////////////

protected:

	void SetType( const grEnums::EntityType type ) { m_Type = type; }

	//////////////////////////////////////////////////

private:

	void EnableDisableRecursively( grIEntity& rEntity );
	void UpdatePosRecursively( grIEntity& rEntity );
	void DestroyRecursively( grIEntity& rEntity );
	inline void ResetPosOnRelease( void );

	//////////////////////////////////////////////////

protected:

	vecIEntity			m_vecChildren;

	grV2f				m_PosWorld,
						m_PosLocal;

	str					m_Name;

	uInt				m_Id;

	grEnums::EntityType	m_Type;
	
	bool				m_bIsEnabled;

	grEntityManager&	rEMan;

	grIEntity*			m_pParent;

};

#endif		//_GRIENTITY_H_

