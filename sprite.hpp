#pragma once
#include "pos.hpp"

class sprite {

public:

    pos Pos{};
    pos Motion{};
    double Direction = 0;   // 0:Right PI/2:Down PI:Left 3PI/2:Up
    pos Size{};

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

};
