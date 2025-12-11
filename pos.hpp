#pragma once
#include <cmath>

/**
 * @brief 2D position/vector class with double precision
 */
class pos {

private:

    double X, Y;

public:

    /**
     * @brief Set both X and Y coordinates
     */
    void SetPos(double X, double Y) {
        this->X = X;
        this->Y = Y;
    }

    /**
     * @brief Set X coordinate
     */
    void SetX(double X) {
        this->X = X;
    }

    /**
     * @brief Set Y coordinate
     */
    void SetY(double Y) {
        this->Y = Y;
    }

    /**
     * @brief Add to both coordinates
     */
    void AddPos(double X, double Y) {
        this->X += X;
        this->Y += Y;
    }

    /**
     * @brief Add to X coordinate
     */
    void AddX(double X) {
        this->X += X;
    }

    /**
     * @brief Add to Y coordinate
     */
    void AddY(double Y) {
        this->Y += Y;
    }

    /**
     * @brief Get X coordinate
     */
    double GetX() const {
        return this->X;
    }

    /**
     * @brief Get Y coordinate
     */
    double GetY() const {
        return this->Y;
    }

    /**
     * @brief Get X coordinate as integer with rounding mode
     * @param Mode 0: round to nearest, 1: floor, 2: ceil
     * @return Rounded integer value
     */
    long GetXInt(int Mode = 0) const {
        switch (Mode) {
        case 0: // Round to nearest
            return static_cast<long>(std::round(this->X));
        case 1: // Floor
            return static_cast<long>(std::floor(this->X));
        case 2: // Ceil
            return static_cast<long>(std::ceil(this->X));
        default:
            return static_cast<long>(std::round(this->X));
        }
    }

    /**
     * @brief Get Y coordinate as integer with rounding mode
     * @param Mode 0: round to nearest, 1: floor, 2: ceil
     * @return Rounded integer value
     */
    long GetYInt(int Mode = 0) const {
        switch (Mode) {
        case 0: // Round to nearest
            return static_cast<long>(std::round(this->Y));
        case 1: // Floor
            return static_cast<long>(std::floor(this->Y));
        case 2: // Ceil
            return static_cast<long>(std::ceil(this->Y));
        default:
            return static_cast<long>(std::round(this->Y));
        }
    }

    /**
     * @brief Construct position with X,Y coordinates
     */
    pos(double X = 0.0, double Y = 0.0) {
        this->SetPos(X, Y);
    }
    
};

/**
 * @brief Calculate distance between two positions
 * @param Pos1 First position
 * @param Pos2 Second position
 * @return Euclidean distance
 */
static double Distance2d(const pos& Pos1, const pos& Pos2) {
    return std::sqrt(
        std::pow(Pos1.GetX() - Pos2.GetX(), 2) +
        std::pow(Pos1.GetY() - Pos2.GetY(), 2)
    );
}
