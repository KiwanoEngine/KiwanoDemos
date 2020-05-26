// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class ShapeDemo
	: public Stage
{
	int current_type = 0;

public:
	static StagePtr Create()
	{
		return new ShapeDemo;
	}

	static String DemoName()
	{
		return "Shape Demo";
	}

	ShapeDemo()
	{
		// �Զ�������״�Ķ�ʱ����
		TaskPtr task = Task::Create(Closure(this, &ShapeDemo::GenerateShapes), 120_msec);
		AddTask(task);
	}

	void GenerateShapes(Task* task, Duration dt)
	{
		const int max_types = 4;

		Point pos = Point(GetWidth() * (current_type + 1) / (max_types + 1), GetHeight() + 50);
		switch (current_type)
		{
		case 0:
		{
			AddShape(CreateRectangle(), pos);
			current_type++;
			break;
		}
		case 1:
		{
			AddShape(CreateEllipse(), pos);
			current_type++;
			break;
		}
		case 2:
		{
			AddShape(CreatePolygon(), pos);
			current_type++;
			break;
		}
		case 3:
		{
			AddShape(CreateTriangle(), pos);
			current_type = 0;
			break;
		}
		}
	}

	// ��������
	ShapeActorPtr CreateRectangle()
	{
		RectActorPtr rect = RectActor::Create(Size(40, 40));
		return rect;
	}

	// ������Բ��
	ShapeActorPtr CreateEllipse()
	{
		EllipseActorPtr ellipse = EllipseActor::Create(Vec2(30, 20));
		return ellipse;
	}

	// ���������
	ShapeActorPtr CreatePolygon()
	{
		// ���������ε���������
		Vector<Point> vertices;
		float radius = 30.0f;
		for (int i = 0; i < 6; i++)
		{
			float s = math::Sin(180.0f - i * 60.0f);
			float c = math::Cos(180.0f - i * 60.0f);
			Point vertex = Point(radius * (1 + c), radius * (1 - s));
			vertices.push_back(vertex);
		}

		PolygonActorPtr polygon = PolygonActor::Create(vertices);
		return polygon;
	}

	// ����������
	ShapeActorPtr CreateTriangle()
	{
		// �ȱ������α߳�
		float side = 45.f;

		// ʹ����״����������������������
		ShapeMakerPtr maker = ShapeMaker::Create();
		maker->BeginPath(Point(0, 0));
		maker->AddLine(Point(side, 0));
		maker->AddLine(Point(side / 2, side * math::Cos(30.0f)));
		maker->EndPath(true);

		// ��ȡ���ɵ���״
		ShapePtr shape = maker->GetShape();
		// ������״��ɫ
		ShapeActorPtr triangle = ShapeActor::Create(shape);
		return triangle;
	}

	// ����״��ӵ�������
	void AddShape(ShapeActorPtr shape, const Point& position)
	{
		// ������״�����ɫ
		shape->SetFillColor(Color(Color::Blue, 0.5f));
		// ������״������ɫ
		shape->SetStrokeColor(Color(Color::Blue, 0.8f));
		// �ӿ���״�������
		shape->SetStrokeStyle(StrokeStyle::Create(1.3f));

		// ��Ӷ���
		shape->AddAction(Tween::MoveTo(4_sec, Point(position.x, -50)).RemoveTargetWhenDone());
		shape->AddAction(Tween::RotateBy(1_sec, math::Random(40.0f, 120.0f)).SetLoops(-1));

		shape->SetRotation(math::Random(0.0f, 360.0f));
		shape->SetPosition(position);
		shape->SetAnchor(0.5f, 0.5f);
		this->AddChild(shape);
	}
};
