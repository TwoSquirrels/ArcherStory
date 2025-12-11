#pragma once
#include "sprite.hpp"
#include "map.hpp"
#include "player.hpp"
#include "monsters/ball.hpp"
#include <map>
#include <string>

class player;
class map;

/**
 * @brief Base monster class with common behavior
 * 
 * This class provides common functionality for all enemy types including
 * health management, collision, knockback, and poison effects.
 */
class monster {

private:

    player *Player;
    map *Map;

    int MaxAttackCount;
    int BeforeHP = this->MaxHP;

    // Poison status effect
    int PoisonLeft = 0;
    int PoisonCount = 0;
    int PoisonDamage;

    // Knockback effect
    int KnockBackCount = 0;
    pos KnockBack{};

    pos SpawnPoint{};

    std::map<std::string, int> Graph;
    std::map<std::string, pos> GraphSize;

public:

    bool Use;          // Whether this monster is active
    bool Hide = false; // Whether monster is hidden (no collision/rendering)

    sprite Sprite;
    int HP, MaxHP;
    
    /**
     * @brief Block collision flags by block type
     */
    std::vector<bool> BlockCol = {
        false,  // air - no collision
        true,   // wall - collides
        true,   // stone - collides
        true,   // pond - collides
    };

    int AttackCount;

    /**
     * @brief Update monster state (movement, collision, effects)
     */
    void Update();

    /**
     * @brief Draw targeting ring below monster
     * @param Scroll Horizontal scroll offset
     */
    void DrawRing(int Scroll);
    
    /**
     * @brief Draw health bar above monster
     * @param Scroll Horizontal scroll offset
     */
    void DrawHP(int Scroll);

    /**
     * @brief Check if monster can attack and reset attack cooldown if specified
     * @param ResetAttackCount Value to reset attack count to (0 = no reset)
     * @return true if attack cooldown is ready
     */
    bool GetAttack(int ResetAttackCount = 0);
    
    /**
     * @brief Check if monster is dead
     * @return true if HP <= 0
     */
    bool GetDeath();

    /**
     * @brief Get current HP
     */
    int GetHP() const;
    
    /**
     * @brief Heal monster by specified amount (capped at MaxHP)
     * @param AddHP Amount to heal
     */
    void Heal(int AddHP);
    
    /**
     * @brief Damage monster and apply knockback
     * @param Damage Damage amount
     * @param Motion Knockback vector
     * @param Poison Whether this is poison damage
     */
    void Damage(int Damage, pos Motion, bool Poison = false);

    /**
     * @brief Hit detection shape
     */
    enum shape {
        SQUARE,  // Axis-aligned bounding box
        CIRCLE   // Circle collision
    };
    
    /**
     * @brief Check collision with another sprite
     * @param Sprite Sprite to check against
     * @param Shape Collision shape to use
     * @return true if collision detected
     */
    bool CheckHit(sprite Sprite, enum shape Shape = SQUARE);

    /**
     * @brief Default constructor
     */
    monster();
    
    /**
     * @brief Construct monster with parameters
     * @param Pos Initial position
     * @param HP Initial and maximum HP
     * @param StartAttackCount Initial attack cooldown
     * @param Map Map reference for collision
     * @param Player Player reference for damage
     * @param Graph Graphics handles map
     * @param Size Sprite size
     */
    monster(pos Pos, int HP, int StartAttackCount, map *Map, player *Player, 
            std::map<std::string, int> Graph, pos Size = pos(32, 32));

};
