#pragma once
#include "pos.hpp"
#include <cmath>

/**
 * @brief Represents a game sprite with position, motion, direction and size
 */
class sprite {

public:

    pos Pos{};
    pos Motion{};
    double Direction = 0.0;   // Radians: 0=Right, PI/2=Down, PI=Left, 3PI/2=Up
    pos Size{};

    /**
     * @brief Move sprite by its motion vector
     * @return New position after movement
     */
    pos Move() {
        this->Pos.AddPos(this->Motion.GetX(), this->Motion.GetY());
        return this->Pos;
    }

    /**
     * @brief Set direction from a position vector
     * @param Pos Position vector to calculate direction from
     * @return Calculated direction in radians
     */
    double SetDrectionFromPos(const pos& Pos) {
        this->Direction = std::atan2(Pos.GetY(), Pos.GetX());
        return this->Direction;
    }
    
    /**
     * @brief Get position vector from current direction
     * @param Radius Length of the vector (default: 1.0)
     * @return Position vector in the sprite's direction
     */
    pos GetPosFromDirection(double Radius = 1.0) const {
        return pos(
            Radius * std::cos(this->Direction),
            Radius * std::sin(this->Direction)
        );
    }

    /**
     * @brief Get center position of sprite
     * @return Center position
     */
    pos GetCenterPos() const {
        return pos(
            this->Pos.GetX() + this->Size.GetX() / 2.0,
            this->Pos.GetY() + this->Size.GetY() / 2.0
        );
    }

    /**
     * @brief Direction enumeration for sprite sides
     */
    enum direction {
        UP, DOWN, LEFT, RIGHT
    };
    
    /**
     * @brief Get position of a specific side of the sprite
     * @param Direction Which side to get
     * @return Position value of that side
     */
    double GetSidePos(enum direction Direction) const {
        switch (Direction) {
        case UP:
            return this->Pos.GetY();
        case DOWN:
            return this->Pos.GetY() + this->Size.GetY();
        case LEFT:
            return this->Pos.GetX();
        case RIGHT:
            return this->Pos.GetX() + this->Size.GetX();
        default:
            return 0.0;
        }
    }

};
