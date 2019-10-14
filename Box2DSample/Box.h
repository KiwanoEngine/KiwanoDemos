// Copyright (C) 2019 Nomango

#pragma once

#include "common.h"

// 方块
KGE_DECLARE_SMART_PTR(Box);
class Box
	: public Sprite
{
public:
	Box(b2World* world, const Point& pos)
	{
		Load(L"resources/box.png");
		SetAnchor(0.5f, 0.5f);

		// 创建盒子的物理形状
		b2PolygonShape shape;
		b2Vec2 sz = Stage2World(GetSize() / 2);
		shape.SetAsBox(sz.x, sz.y);

		// 创建盒子的密度和摩擦力
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		// 创建盒子在物理世界的位置角度信息
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = Stage2World(pos);

		// 创建盒子的物理世界实体
		b2Body* body = world->CreateBody(&bodyDef);
		body->CreateFixture(&fixtureDef);
		body->SetUserData(this);
	}
};
