#include "Transform.h"

void Transform::Update()
{
	if (m_npc)
		m_position = m_npc->getPosition();
}

sf::Vector2f Transform::getPosition() const
{
	return m_npc ? m_npc->getPosition() : m_position;
}

void Transform::setPosition(const sf::Vector2f& pos)
{
    if (m_npc)
        m_npc->setPosition(pos);

    else
        m_position = pos;
}