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
    const float PI = 3.14159265f;
    sf::Vector2f playerPos = m_context.getPlayerPos();

    for (size_t i = 0; i < m_npcs.size(); ++i)
    {
        auto& npc = m_npcs[i];

        sf::Vector2f toPlayer = playerPos - npc->getPosition();
        float distance = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

        bool playerVisible = false;

        float visionLen = npc->getVisionLength();
        if (distance <= visionLen && distance > 0.0001f)
        {
            // normalize
            sf::Vector2f toPlayerNorm = toPlayer / distance;

            // compute NPC forward vector using the same rotation basis as updateVisionCone()
            // (rotationDegrees - 90) is used in updateVisionCone to derive direction
            float rotationDeg = npc->getRotationDegrees() - 90.f;
            float rotationRad = rotationDeg * PI / 180.f;
            sf::Vector2f forward(std::cos(rotationRad), std::sin(rotationRad));

            // dot product to get cos(angle between forward and toPlayer)
            float dot = forward.x * toPlayerNorm.x + forward.y * toPlayerNorm.y;

            // compare against cos(halfConeAngle)
            float cosHalf = std::cos(npc->getVisionHalfAngleRadians());
            if (dot >= cosHalf)
                playerVisible = true;
        }

        // set detection BEFORE Update, so Update -> updateVisionCone() can pick up the flag and colour it.
        npc->setPlayerDetected(playerVisible);

        // optional debug:
        // std::cout << "NPC " << i << " detected=" << playerVisible << " dist=" << distance << "\n";

        npc->Update(dt);
    }
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


