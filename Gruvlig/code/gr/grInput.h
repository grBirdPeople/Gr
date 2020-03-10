#ifndef		_GRINPUT_H_
#define		_GRINPUT_H_

#include	<SFML/Window/Keyboard.hpp>	

#include	"grCommon.h"
#include	"grSingleton.h"


// grInput
//////////////////////////////////////////////////
class grInput : public grSingleton<grInput>
{
public:

					grInput		(void);
					~grInput	(void)					{ ResetKeys(); }

	//////////////////////////////////////////////////

	inline	bool	GetKeyDown	(sf::Keyboard::Key key)	{ return m_Keys[key]; }
	inline	void	SetKeyDown	(sf::Keyboard::Key key)	{ m_Keys[key] = true; }
	inline	void	SetKeyUp	(sf::Keyboard::Key key)	{ m_Keys[key] = false; }
	
	//////////////////////////////////////////////////
	
	inline	void	ResetKeys	(void)
	{
		for (uInt i = 0; i < m_KeyCount; i++)
		{
			m_Keys[i] = false;
		}
	}

	//////////////////////////////////////////////////

private:

	std::vector<bool>	m_Keys;
	uInt				m_KeyCount;
};

#endif		//_GRINPUT_H_

