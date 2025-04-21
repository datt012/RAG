#pragma once

class CharacterDirection {
public:
    enum Value {
        LEFT,
        RIGHT
    };

    CharacterDirection() = default;
    explicit CharacterDirection(Value value) : m_Value(value) {}

    operator Value() const { return m_Value; }
    bool operator==(CharacterDirection other) const { return m_Value == other.m_Value; }
    bool operator!=(CharacterDirection other) const { return m_Value != other.m_Value; }

private:
    Value m_Value = RIGHT; // Default value
};
