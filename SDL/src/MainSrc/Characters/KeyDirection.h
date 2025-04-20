#pragma once

enum KeyDirection {
	KEY_LEFT = 1 << 0, // 0001
	KEY_DOWN = 1 << 1, // 0010
	KEY_RIGHT = 1 << 2, // 0100
	KEY_UP = 1 << 3,  // 1000
	KEY_SHOOT = 1 << 4,
	KEY_JUMP = 1 << 5,
};