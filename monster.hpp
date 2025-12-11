#pragma once
#include "sprite.hpp"
#include "map.hpp"
#include "player.hpp"
#include "monsters/ball.hpp"

class player;
class map;

class monster {

private:

    player *Player;
    map *Map;

    int MaxAttackCount;
    int BeforeHP = this->MaxHP;

    int PoisonLeft = 0;
    int PoisonCount = 0;
    int PoisonDamage;

    int KnockBackCount = 0;
    pos KnockBack{};

    pos SpawnPoint{};

    std::map<std::string, int> Graph;
    std::map<std::string, pos> GraphSize;

public:

    bool Use;
    bool Hide = false;

    sprite Sprite;
    int HP, MaxHP;
    std::vector<bool> BlockCol = {
        false,  // air
        true,   // wall
        true,   // stone
        true,   // pond
    };

    int AttackCount;

    void Update();

    void DrawRing(int Scroll);
    void DrawHP(int Scroll);

    bool GetAttack(int ResetAttackCount = 0);
    bool GetDeath();

    int GetHP();
    void Heal(int AddHP);
    void Damage(int Damage, pos Motion, bool Poison = false);

    enum shape {
        SQUARE, CIRCLE
    };
    bool CheckHit(sprite Sprite, enum shape Shape = SQUARE);

    monster();
    monster(pos Pos, int HP, int StartAttackCount, map *Map, player *Player, std::map<std::string, int> Graph, pos Size = pos(32, 32));

};
