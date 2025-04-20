#pragma once

class DirectionGun {
public:
    enum Value {
        DIR_HORIZONTAL,
        DIR_UP,
        DIR_DOWN,
        DIR_DIAGONAL_UP,
        DIR_DIAGONAL_DOWN
    };

    DirectionGun() = default;
    explicit DirectionGun(Value value) : m_Value(value) {}

    operator Value() const { return m_Value; }
    bool operator==(DirectionGun other) const { return m_Value == other.m_Value; }
    bool operator!=(DirectionGun other) const { return m_Value != other.m_Value; }

private:
    Value m_Value = DIR_HORIZONTAL; // Default value
};
