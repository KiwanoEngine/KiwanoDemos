// Copyright (C) 2019 Nomango

#pragma once

#include "common.h"

// �̶���ľ��
KGE_DECLARE_SMART_PTR(Board);
class Board
	: public RectActor
{
public:
	Board(b2World* world, const Size& size, const Point& pos)
	{
		// ���������������ɫ
		Color color = 0x868686;
		SetStrokeColor(color);
		SetFillColor(color);

		// ����ľ��Ĵ�С��λ�ú���ת�Ƕ�
		SetRectSize(size);
		SetAnchor(0.5f, 0.5f);
		SetRotation(10);
		SetPosition(pos);

		// ����ľ���������״
		b2PolygonShape groundBox;
		b2Vec2 sz = Stage2World(size / 2);
		groundBox.SetAsBox(sz.x, sz.y);

		// ����ľ�������������λ�ýǶ���Ϣ
		b2BodyDef groundBodyDef;
		groundBodyDef.position = Stage2World(GetPosition());
		groundBodyDef.angle = Angle2Radian(10);

		// ����ľ�����������ʵ��
		b2Body* groundBody = world->CreateBody(&groundBodyDef);
		groundBody->CreateFixture(&groundBox, 0.0f);
	}
};
