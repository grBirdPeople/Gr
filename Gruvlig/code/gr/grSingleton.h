#ifndef	_GRCSINGLETON_H_
#define	_GRCSINGLETON_H_

#include	"grCommon.h"
#include	"grDefine.h"


// grSingleton
//////////////////////////////////////////////////
template <typename T>
class grSingleton
{
protected:

	grSingleton( void ) {}
	virtual	~grSingleton( void ) {}

	//////////////////////////////////////////////////

public:

	static inline T& Initialize (void) { assert(m_pInstance == nullptr && "grSingleton::Initialize(): Singleton already initialized"); m_pInstance = new T; return *m_pInstance; }
	static inline void DeInitialize	(void) { assert(m_pInstance != nullptr && "grSingleton::Deinitialize(): No singleton initialized"); DELANDNULL(m_pInstance); }
	static inline T& Instance (void) { assert(m_pInstance != nullptr && "grSingleton::Instance(): No singleton initialized"); return *m_pInstance; }
	static inline T* InstancePtr (void) { return m_pInstance; }

	//////////////////////////////////////////////////

private:

	static T* m_pInstance;

};

template<typename T>
T*
grSingleton<T>::m_pInstance = nullptr;

#endif // _GRCSINGLETON_H_
