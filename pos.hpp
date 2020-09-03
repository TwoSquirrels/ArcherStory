#pragma once

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

    double GetX() {
        return this->X;
    }

    double GetY() {
        return this->Y;
    }

    pos(double X = 0.0, double Y = 0.0) {
        this->SetPos(X, Y);
    }
    
};

