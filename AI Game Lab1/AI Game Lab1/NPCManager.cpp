#include "NPCManager.h"
#include <iostream>

NPCManager::NPCManager(PlayerContext& context)
    : m_context(context)
{
    createNPCs(m_context);
}


void NPCManager::createNPCs(const PlayerContext& context)
{
    // Preset positions
    std::vector<sf::Vector2f> positions = {
        {400,300}, {500,300}, {600,300}, {700,300}, {800,300}
    };

    // Preset speeds
    std::vector<float> speeds = { 50, 60, 40, 55, 45 };

    for (size_t i = 0; i < positions.size(); ++i)
    {
        auto npc = std::make_unique<NPC>(positions[i]);

        // Assign behaviour based on type
        if (i == 0) npc->setBehavior(std::make_unique<SeekBehavior>(context), "Seek");
        else if (i == 1) npc->setBehavior(std::make_unique<WanderBehavior>(context), "Wander");
        else if (i == 2) npc->setBehavior(std::make_unique<PursueBehavior>(context), "Pursue");
        else npc->setBehavior(std::make_unique<ArriveBehavior>(context), "Arrive");

        // Optional: set individual speeds
        npc->setSpeed(speeds[i]);

        m_npcs.push_back(std::move(npc));
    }
}

void NPCManager::updateAll(float dt)
{
    for (auto& npc : m_npcs)
        npc->Update(dt);
}

void NPCManager::render(sf::RenderWindow& window)
{
    for (auto& npc : m_npcs)
    {
        npc->Render(window);
    }
}

void NPCManager::toggleNPC(size_t index)
{
    if (index < m_npcs.size())
    {
        m_npcs[index]->toggleActive();
        std::cout << "NPC " << index + 1 << " toggled.\n";
    }
}

std::vector<std::unique_ptr<NPC>>& NPCManager::getAll()
{
    return m_npcs;
}


