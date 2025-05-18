#pragma once
#include <memory>
#include "ARMob.h"
#include "SniperMob.h"
#include "RPGMob.h"
#include "Boss1.h"
class Behavior {
public:
	static int GenerateKeyMask(std::shared_ptr<Enemy> enemy, std::shared_ptr<Map> map);
	static int GenerateKeyMask(std::shared_ptr<ARMob> armob, std::shared_ptr<Map> map);
	static int GenerateKeyMask(std::shared_ptr<SniperMob> sniperMob, std::shared_ptr<Map> map);
	static int GenerateKeyMask(std::shared_ptr<RPGMob> rpgMob, std::shared_ptr<Map> map);
	static int GenerateKeyMask(std::shared_ptr<Boss1> boss1, std::shared_ptr<Map> map);
};