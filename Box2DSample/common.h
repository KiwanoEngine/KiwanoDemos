// Copyright (C) 2019 Nomango

#pragma once

#include <kiwano/kiwano.h>
#include <kiwano-physics/kiwano-physics.h>

using namespace kiwano;

//
// Box2D 世界以 米/千克/秒 为单位, 并不是以像素为单位进行计算.
// 我们在平衡 Box2D 和 Kiwano 世界的度量时, 需要进行一些转换.
// 如果不进行转换, 1 像素在 Box2D 看来就像一米这么长, 模拟的物理
// 环境会非常辽阔, 所以我们使用 100像素/米 的比例进行转换
//
namespace
{
	const float GLOBAL_SCALE = 100.0f;

	inline float World2Stage(float value)
	{
		return value * GLOBAL_SCALE;
	}

	inline float Stage2World(float value)
	{
		return value / GLOBAL_SCALE;
	}

	inline b2Vec2 Stage2World(const Point& pos)
	{
		return b2Vec2(Stage2World(pos.x), Stage2World(pos.y));
	}

	inline Point World2Stage(const b2Vec2& pos)
	{
		return Point(World2Stage(pos.x), World2Stage(pos.y));
	}

	inline float Angle2Radian(float angle)
	{
		return angle * math::constants::PI_F / 180.f;
	}

	inline float Radian2Angle(float radian)
	{
		return radian * 180.f / math::constants::PI_F;
	}
}
