#pragma once

class CharacterAction {
public:
    enum Value {
        IDLE,
        RUN,
        CROUCH,
        FALL
    };

    CharacterAction() = default;
    explicit CharacterAction(Value value) : m_Value(value) {}

    operator Value() const { return m_Value; }
    bool operator==(CharacterAction other) const { return m_Value == other.m_Value; }
    bool operator!=(CharacterAction other) const { return m_Value != other.m_Value; }

private:
    Value m_Value = IDLE; // Default value
};
