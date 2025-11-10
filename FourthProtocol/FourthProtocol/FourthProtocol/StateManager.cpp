#include "StateManager.h"


StateManager::StateManager()
{
}

void StateManager::changeState(std::unique_ptr<GameState> t_newState)
{
    if (m_currentState)
    {
        m_currentState->onExit();
    }

    m_currentState = std::move(t_newState);

    if (m_currentState)
    {
        m_currentState->onEnter();
    }
}

void StateManager::update(sf::Time t_deltaTime)
{
    if (!m_currentState)
    {
        return;
    }

    m_currentState->update(t_deltaTime);

    // Check if state wants to transition
    if (m_currentState->shouldTransition())
    {
        changeState(m_currentState->getNextState());
    }
}

void StateManager::render(sf::RenderWindow& t_window)
{
    if (!m_currentState)
    {
        return;
    }

    m_currentState->render(t_window);
}

void StateManager::handleEvent(const sf::Event& t_event)
{
    if (!m_currentState)
    {
        return;
    }

    m_currentState->handleEvent(t_event);
}

bool StateManager::hasNoState() const
{
    return m_currentState == nullptr;
}