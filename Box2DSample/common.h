// Copyright (C) 2019 Nomango

#pragma once

#include <kiwano/kiwano.h>
#include <kiwano-physics/kiwano-physics.h>

using namespace kiwano;

//
// Box2D ������ ��/ǧ��/�� Ϊ��λ, ������������Ϊ��λ���м���.
// ������ƽ�� Box2D �� Kiwano ����Ķ���ʱ, ��Ҫ����һЩת��.
// ���������ת��, 1 ������ Box2D ��������һ����ô��, ģ�������
// ������ǳ�����, ��������ʹ�� 100����/�� �ı�������ת��
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
