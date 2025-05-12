#pragma once
#include <memory>
#include "ARMob.h"

class Behavior
{
public:
	static int GenerateKeyMask(std::shared_ptr<Enemy> enemy, std::shared_ptr<Map> map);
	static int GenerateKeyMask(std::shared_ptr<ARMob> armob, std::shared_ptr<Map> map);
};