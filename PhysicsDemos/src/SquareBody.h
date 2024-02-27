// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class Square
	: public ShapeActor
{
public:
	RefPtr<physics::Body> body;

	Square(RefPtr<physics::World> world, const Point& pos, const Size& size)
	{
		// ������״����ɫ
		this->SetShape(Shape::CreateRect(Rect(0, 0, size.x, size.y)));
		this->SetStrokeColor(Color::White);

		// ����λ�ú�ê��
		this->SetAnchor(0.5f, 0.5f);
		this->SetPosition(pos);
		this->SetSize(size);

		// ������������
		b2BodyDef def;
		def.type = b2_dynamicBody;
		this->body = world->AddBody(&def);
		this->AddComponent(body);

		b2PolygonShape shape;
		b2Vec2 b2size = physics::LocalToWorld(this->GetSize());
		shape.SetAsBox(b2size.x / 2, b2size.y / 2);

		b2FixtureDef fd;
		fd.density = 1.0f;
		fd.friction = 0.2f;
		fd.shape = &shape;
		body->GetB2Body()->CreateFixture(&fd);
	}
};
