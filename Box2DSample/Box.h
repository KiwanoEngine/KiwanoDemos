// Copyright (C) 2019 Nomango

#pragma once

#include "common.h"

// ����
KGE_DECLARE_SMART_PTR(Box);
class Box
	: public Sprite
{
public:
	Box(b2World* world, const Point& pos)
	{
		Load(L"resources/box.png");
		SetAnchor(0.5f, 0.5f);

		// �������ӵ�������״
		b2PolygonShape shape;
		b2Vec2 sz = Stage2World(GetSize() / 2);
		shape.SetAsBox(sz.x, sz.y);

		// �������ӵ��ܶȺ�Ħ����
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		// �������������������λ�ýǶ���Ϣ
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = Stage2World(pos);

		// �������ӵ���������ʵ��
		b2Body* body = world->CreateBody(&bodyDef);
		body->CreateFixture(&fixtureDef);
		body->SetUserData(this);
	}
};
