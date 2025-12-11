#include "monster.hpp"
#include <algorithm>

void monster::Update() {

    // Clamp HP to maximum
    if (this->HP > this->MaxHP) {
        this->HP = this->MaxHP;
    }
    
    // Update poison effect
    constexpr int POISON_TICK_FRAMES = 60; // Apply poison damage once per second
    if (this->PoisonLeft > 0) {
        this->PoisonCount--;
        if (this->PoisonCount <= 0) {
            this->PoisonCount = POISON_TICK_FRAMES;
            this->PoisonLeft--;
            this->Damage(this->PoisonDamage, pos(0, 0));
        }
    }
    
    // Update attack cooldown
    if (this->AttackCount > 0) {
        this->AttackCount--;
    }

    // Handle knockback or normal movement
    if (this->KnockBackCount > 0) {
        // Apply knockback
        this->KnockBackCount--;
        this->Sprite.Pos.AddPos(this->KnockBack.GetX(), this->KnockBack.GetY());
        
        // Collision detection with knockback
        pos Motion = this->Sprite.Motion;
        this->Sprite.Motion = this->KnockBack;
        this->Map->Collision(&(this->Sprite), BlockCol);
        this->Sprite.Motion = Motion;
    } else {
        // Normal movement
        this->Sprite.Move();
        
        // Collision detection
        this->Map->Collision(&(this->Sprite), BlockCol);
    }
    
    // Keep monster within map bounds
    if (!this->Map->GetInMap(this->Sprite)) {
        this->Sprite.Pos = this->SpawnPoint;
    }
    
    // Contact damage to player
    constexpr int CONTACT_DAMAGE_DIVISOR = 64;
    if (!this->Hide && this->Player->CheckHit(this->Sprite, player::CIRCLE)) {
        this->Player->Damage(this->Player->GetMaxHP() / CONTACT_DAMAGE_DIVISOR);
    }

}

void monster::DrawRing(int Scroll) {
    if (this->Hide) {
        return;
    }

    constexpr int Y_OFFSET = 96; // Vertical offset for rendering
    const int ringSize = std::min(
        this->Sprite.Size.GetXInt(), 
        this->Sprite.Size.GetYInt()
    );
    const int centerX = this->Sprite.GetCenterPos().GetXInt();
    const int centerY = this->Sprite.GetCenterPos().GetYInt();

    DxLib::DrawExtendGraph(
        -Scroll + centerX - ringSize / 2,
        Y_OFFSET + centerY - ringSize / 2,
        -Scroll + centerX + ringSize / 2,
        Y_OFFSET + centerY + ringSize / 2,
        this->Graph["ring"], TRUE
    );
}

void monster::DrawHP(int Scroll) {
    if (this->Hide) {
        return;
    }

    constexpr int Y_OFFSET = 96;
    constexpr int HP_BAR_WIDTH = 64;
    constexpr int HP_BAR_HEIGHT = 8;
    constexpr int HP_BAR_Y_OFFSET = 16;
    constexpr int HP_TEXT_Y_OFFSET = 32;
    
    const int centerX = this->Sprite.GetCenterPos().GetXInt();
    const int topY = this->Sprite.Pos.GetYInt();
    const int barLeft = -Scroll + centerX - HP_BAR_WIDTH / 2;
    const int barRight = -Scroll + centerX + HP_BAR_WIDTH / 2;
    const int barTop = Y_OFFSET + topY - HP_BAR_Y_OFFSET;
    const int barBottom = Y_OFFSET + topY - HP_BAR_Y_OFFSET + HP_BAR_HEIGHT;

    // Draw black background
    DxLib::DrawBox(barLeft, barTop, barRight, barBottom, 0x000000, TRUE);
    
    // Draw red health bar
    const int healthWidth = 1 + (HP_BAR_WIDTH - 1) * this->HP / this->MaxHP;
    DxLib::DrawBox(
        barLeft, barTop, 
        barLeft + healthWidth, barBottom,
        0xff0000, TRUE
    );
    
    // Draw border
    DxLib::DrawBox(barLeft, barTop, barRight, barBottom, 0x000000, FALSE);
    
    // Draw HP number
    DxLib::DrawFormatString(
        barLeft, Y_OFFSET + topY - HP_TEXT_Y_OFFSET,
        0x000000, "%d", this->HP
    );
}

bool monster::GetAttack(int ResetAttackCount) {
    if (this->AttackCount == 0) {
        this->AttackCount = (ResetAttackCount == 0) ? this->MaxAttackCount : ResetAttackCount;
        return true;
    }
    return false;
}

bool monster::GetDeath() {
    bool Death = (this->BeforeHP > 0 && HP <= 0);
    this->BeforeHP = this->HP;
    return Death;
}

int monster::GetHP() const {
    return this->HP;
}

void monster::Heal(int AddHP) {
    this->HP += AddHP;
    if (this->HP > this->MaxHP) {
        this->HP = this->MaxHP;
    }
}

void monster::Damage(int Damage, pos Motion, bool Poison) {
    constexpr int KNOCKBACK_DIVISOR = 4;
    constexpr int KNOCKBACK_FRAMES = 4;
    constexpr int ATTACK_DELAY_ON_HIT = 10;
    constexpr int POISON_DURATION = 8;
    constexpr int POISON_DAMAGE_DIVISOR = 4;
    constexpr int POISON_TICK_FRAMES = 60;
    
    this->HP -= Damage;
    this->AttackCount += ATTACK_DELAY_ON_HIT;
    
    // Apply knockback
    this->KnockBack = pos(
        Motion.GetX() / KNOCKBACK_DIVISOR, 
        Motion.GetY() / KNOCKBACK_DIVISOR
    );
    this->KnockBackCount = KNOCKBACK_FRAMES;
    
    // Apply poison effect
    if (Poison) {
        if (this->PoisonLeft == 0) {
            this->PoisonCount = POISON_TICK_FRAMES;
        }
        this->PoisonLeft = POISON_DURATION;
        this->PoisonDamage = Damage / POISON_DAMAGE_DIVISOR;
    }
    
    // Check for death
    if (this->HP <= 0) {
        this->HP = 0;
        this->Use = false;
    }
}

bool monster::CheckHit(sprite Sprite, enum shape Shape) {

    switch (Shape) {
    case SQUARE:
        // Axis-aligned bounding box collision
        if (this->Sprite.GetSidePos(sprite::LEFT) >= Sprite.GetSidePos(sprite::RIGHT)) return false;
        if (this->Sprite.GetSidePos(sprite::RIGHT) <= Sprite.GetSidePos(sprite::LEFT)) return false;
        if (this->Sprite.GetSidePos(sprite::UP) >= Sprite.GetSidePos(sprite::DOWN)) return false;
        if (this->Sprite.GetSidePos(sprite::DOWN) <= Sprite.GetSidePos(sprite::UP)) return false;
        return true;
        
    case CIRCLE:
        // Circle collision using minimum dimension as radius
        {
            const double distance = Distance2d(
                this->Sprite.GetCenterPos(), 
                Sprite.GetCenterPos()
            );
            const double radiusSum = 
                std::min(this->Sprite.Size.GetX(), this->Sprite.Size.GetY()) + 
                std::min(Sprite.Size.GetX(), Sprite.Size.GetY());
            return distance < radiusSum;
        }
        
    default:
        return false;
    }
}

monster::monster() {
    this->Use = false;
}

monster::monster(pos Pos, int HP, int StartAttackCount, map *Map, player *Player, 
                 std::map<std::string, int> Graph, pos Size) {

    this->Use = true;

    this->Sprite.Pos = Pos;
    this->SpawnPoint = this->Sprite.Pos;
    this->MaxHP = HP;
    this->HP = HP;
    this->AttackCount = StartAttackCount;
    this->MaxAttackCount = StartAttackCount;
    this->Map = Map;
    this->Player = Player;
    this->Graph = Graph;
    this->Sprite.Size = Size;
    
    // Get image dimensions
    int X, Y;
    DxLib::GetGraphSize(this->Graph["ring"], &X, &Y);
    this->GraphSize["ring"].SetPos(X, Y);
}
