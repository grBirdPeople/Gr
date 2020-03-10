#include	"grCtrlPlayer.h"

#include	"grDefine.h"
#include	"grIEntity.h"
#include	"grInput.h"


void
grCtrlPlayer::Update(void)
{
	if(GetActiveStatus() == true)
	{
		m_InputDir.x = 0.0f;
		m_InputDir.y = 0.0f;

		grInput& input = grInput::Instance();
		if(input.GetKeyDown(sf::Keyboard::Key::W))	m_InputDir.y -= 1.0f;
		if(input.GetKeyDown(sf::Keyboard::Key::A))	m_InputDir.x -= 1.0f;
		if(input.GetKeyDown(sf::Keyboard::Key::S))	m_InputDir.y += 1.0f;
		if(input.GetKeyDown(sf::Keyboard::Key::D))	m_InputDir.x += 1.0f;

		if ((m_InputDir.x != 0.0f) && (m_InputDir.y != 0.0f))
		{ m_InputDir /= sqrt((m_InputDir.x * m_InputDir.x) + (m_InputDir.y * m_InputDir.y)); }
	}
}
