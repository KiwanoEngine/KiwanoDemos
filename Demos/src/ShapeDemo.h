// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class ShapeDemo
	: public Stage
{
	int current_type = 0;

public:
	static RefPtr<Stage> Create()
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
		RefPtr<Task> task = new Task(Closure(this, &ShapeDemo::GenerateShapes), 120_msec);
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
			break;
		}
		case 1:
		{
			AddShape(CreateEllipse(), pos);
			break;
		}
		case 2:
		{
			AddShape(CreatePolygon(), pos);
			break;
		}
		case 3:
		{
			AddShape(CreateTriangle(), pos);
			break;
		}
		}

		current_type++;
		if (current_type == max_types)
			current_type = 0;
	}

	// ��������
	RefPtr<ShapeActor> CreateRectangle()
	{
		RefPtr<RectActor> rect = new RectActor(Size(40, 40));
		return rect;
	}

	// ������Բ��
	RefPtr<ShapeActor> CreateEllipse()
	{
		RefPtr<EllipseActor> ellipse = new EllipseActor(Vec2(30, 20));
		return ellipse;
	}

	// ���������
	RefPtr<ShapeActor> CreatePolygon()
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

		RefPtr<PolygonActor> polygon = new PolygonActor(vertices);
		return polygon;
	}

	// ����������
	RefPtr<ShapeActor> CreateTriangle()
	{
		// �ȱ������α߳�
		float side = 45.f;

		// ʹ����״����������������������
		RefPtr<ShapeMaker> maker = new ShapeMaker();
		maker->BeginPath(Point(0, 0));
		maker->AddLine(Point(side, 0));
		maker->AddLine(Point(side / 2, side * math::Cos(30.0f)));
		maker->EndPath(true);

		// ��ȡ���ɵ���״
		RefPtr<Shape> shape = maker->GetShape();
		// ������״��ɫ
		RefPtr<ShapeActor> triangle = new ShapeActor(shape);
		return triangle;
	}

	// ����״��ӵ�������
	void AddShape(RefPtr<ShapeActor> shape, const Point& position)
	{
		// ������״�����ɫ
		shape->SetFillColor(Color(Color::Blue, 0.5f));
		// ������״������ɫ
		shape->SetStrokeColor(Color(Color::Blue, 0.8f));
		// �ӿ���״�������
		shape->SetStrokeStyle(new StrokeStyle(1.3f));

		// ��Ӷ���
		shape->StartAnimation(animation::MoveTo(4_sec, Point(position.x, -50)).RemoveTargetWhenDone());
		shape->StartAnimation(animation::RotateBy(1_sec, math::Random(40.0f, 120.0f)).Loops(-1));

		shape->SetRotation(math::Random(0.0f, 360.0f));
		shape->SetPosition(position);
		shape->SetAnchor(0.5f, 0.5f);
		this->AddChild(shape);
	}
};
