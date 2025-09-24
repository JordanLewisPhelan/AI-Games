#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "NPC.h"
#include "Behaviours.h"

class NPCManager
{
public:
    NPCManager() = default;

    // for pointers
    NPCManager(PlayerContext& context);

    void createNPCs(const PlayerContext& context);

    void updateAll(float dt);

    /// for non pointers
    //void addNPC(const sf::Vector2f& pos, SteeringBehavior* defaultBehavior, const std::string& behaviorName);

    //void update(float deltaTime);
    void render(sf::RenderWindow& window);

    void toggleNPC(size_t index);

    std::vector<std::unique_ptr<NPC>>& getAll();

private:
    PlayerContext& m_context;
    std::vector<std::unique_ptr<NPC>> m_npcs;
};
