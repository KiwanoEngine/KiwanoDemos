// Copyright (C) 2019 Nomango

#pragma once

#include "common.h"

// 圆形
KGE_DECLARE_SMART_PTR(Ball);
class Ball
	: public Sprite
{
public:
	Ball(b2World* world, const Point& pos)
	{
		Load(L"resources/ball.png");
		SetAnchor(0.5f, 0.5f);

		// 创建小球的物理形状
		b2CircleShape shape;
		shape.m_radius = Stage2World(GetWidth()) / 2;

		// 创建小球的密度和摩擦力
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		// 创建小球在物理世界的位置角度信息
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = Stage2World(pos);

		// 创建小球的物理世界实体
		b2Body* body = world->CreateBody(&bodyDef);
		body->CreateFixture(&fixtureDef);
		body->SetUserData(this);
	}
};
