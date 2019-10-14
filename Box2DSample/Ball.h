// Copyright (C) 2019 Nomango

#pragma once

#include "common.h"

// Բ��
KGE_DECLARE_SMART_PTR(Ball);
class Ball
	: public Sprite
{
public:
	Ball(b2World* world, const Point& pos)
	{
		Load(L"resources/ball.png");
		SetAnchor(0.5f, 0.5f);

		// ����С���������״
		b2CircleShape shape;
		shape.m_radius = Stage2World(GetWidth()) / 2;

		// ����С����ܶȺ�Ħ����
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		// ����С�������������λ�ýǶ���Ϣ
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = Stage2World(pos);

		// ����С�����������ʵ��
		b2Body* body = world->CreateBody(&bodyDef);
		body->CreateFixture(&fixtureDef);
		body->SetUserData(this);
	}
};
