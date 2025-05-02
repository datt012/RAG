#pragma once
#include "KeyDirection.h"

class DirectionGun {
public:
    enum Value {
        NONE = 0,
        LEFT = 1 << 0,
		RIGHT = 1 << 1,
		UP = 1 << 2,
		DOWN = 1 << 3,
		LEFT_UP = LEFT + UP,
        LEFT_DOWN = LEFT + DOWN,
        RIGHT_UP = RIGHT + UP,
        RIGHT_DOWN = RIGHT + DOWN,
    };

    DirectionGun() = default;
    DirectionGun(Value value) : m_Value(value) {}

    operator Value() const { return m_Value; }
    bool operator==(DirectionGun other) const { return m_Value == other.m_Value; }
    bool operator!=(DirectionGun other) const { return m_Value != other.m_Value; }

    char* to_string() {
		switch (m_Value) {
		case NONE: return "NONE";
		case LEFT: return "LEFT";
		case RIGHT: return "RIGHT";
		case UP: return "UP";
		case DOWN: return "DOWN";
		case LEFT_UP: return "LEFT_UP";
		case LEFT_DOWN: return "LEFT_DOWN";
		case RIGHT_UP: return "RIGHT_UP";
		case RIGHT_DOWN: return "RIGHT_DOWN";
		default: return "UNKNOWN";
		}
    }

	static DirectionGun FromKeyMask(int keyMask) {
        if (keyMask & KEY_LEFT && keyMask & KEY_RIGHT) {
			keyMask -= KEY_LEFT;
			keyMask -= KEY_RIGHT;
        }
        if (keyMask & KEY_UP && keyMask & KEY_DOWN) {
            keyMask -= KEY_UP;
            keyMask -= KEY_DOWN;
        }

		Value horizontal = (keyMask & KEY_LEFT) ? LEFT : (keyMask & KEY_RIGHT) ? RIGHT : NONE;
		Value vertical = (keyMask & KEY_UP) ? UP : (keyMask & KEY_DOWN) ? DOWN : NONE;

		Value value = static_cast<Value>(horizontal + vertical);

		return DirectionGun(value);
	}
private:
    Value m_Value = RIGHT; // Default value
};
