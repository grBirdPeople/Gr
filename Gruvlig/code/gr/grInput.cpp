#include	"grInput.h"

#include	"grDefine.h"


// cTor
//////////////////////////////////////////////////
grInput::grInput(void)
{
	m_KeyCount = sf::Keyboard::KeyCount;
	m_Keys.reserve(m_KeyCount);
	for (uInt i = 0; i < m_KeyCount; i++)
	{
		m_Keys.push_back(false);
	}
}
