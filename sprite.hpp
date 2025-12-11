#pragma once
#include "pos.hpp"

class sprite {

public:

    pos Pos{};
    pos Motion{};
    double Direction = 0;   // 0:Right PI/2:Down PI:Left 3PI/2:Up
    pos Size{};

    pos Move() {
        this->Pos.AddPos(this->Motion.GetX(), this->Motion.GetY());
        return this->Pos;
    }

    double SetDrectionFromPos(pos Pos) {
        this->Direction = std::atan2(Pos.GetY(), Pos.GetX());
        return this->Direction;
    }
    pos GetPosFromDirection(double Radius = 1.0) {
        return pos(
            Radius * std::cos(this->Direction),
            Radius * std::sin(this->Direction)
        );
    }

    pos GetCenterPos() {
        return pos(
            this->Pos.GetX() + this->Size.GetX() / 2,
            this->Pos.GetY() + this->Size.GetY() / 2
        );
    }

    enum direction {
        UP, DOWN, LEFT, RIGHT
    };
    double GetSidePos(enum direction Direction) {
        switch (Direction) {
        case this->UP:
            return this->Pos.GetY();
        case this->DOWN:
            return this->Pos.GetY() + this->Size.GetY();
        case this->LEFT:
            return this->Pos.GetX();
        case this->RIGHT:
            return this->Pos.GetX() + this->Size.GetX();
        }
        return 0;
    }

};
