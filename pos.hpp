#pragma once
#include <cmath>

class pos {

private:

    double X, Y;

public:

    void SetPos(double X, double Y) {
        this->X = X;
        this->Y = Y;
    }

    void SetX(double X) {
        this->X = X;
    }

    void SetY(double Y) {
        this->Y = Y;
    }

    void AddPos(double X, double Y) {
        this->X += X;
        this->Y += Y;
    }

    void AddX(double X) {
        this->X += X;
    }

    void AddY(double Y) {
        this->Y += Y;
    }

    double GetX() {
        return this->X;
    }

    double GetY() {
        return this->Y;
    }

    /*
     * Mode 0:四捨五入 1:切り捨て 2:切り上げ
     */
    long GetXInt(int Mode = 0) {
        switch (Mode) {
        case 0: // 四捨五入
            return (long)std::round(this->X);
        case 1: // 切り捨て
            return (long)std::floor(this->X);
        case 2: // 切り上げ
            return (long)std::ceil(this->X);
        }
        return (long)std::round(this->X);
    }

    /*
     * Mode 0:四捨五入 1:切り捨て 2:切り上げ
     */
    long GetYInt(int Mode = 0) {
        switch (Mode) {
        case 0: // 四捨五入
            return (long)std::round(this->Y);
        case 1: // 切り捨て
            return (long)std::floor(this->Y);
        case 2: // 切り上げ
            return (long)std::ceil(this->Y);
        }
        return (long)std::round(this->Y);
    }

    pos(double X = 0.0, double Y = 0.0) {
        this->SetPos(X, Y);
    }
    
};

// 2点間の距離
static double Distance2d(pos Pos1, pos Pos2) {
    return std::sqrt(
        std::pow(Pos1.GetX() - Pos2.GetX(), 2) +
        std::pow(Pos1.GetY() - Pos2.GetY(), 2)
    );
}
