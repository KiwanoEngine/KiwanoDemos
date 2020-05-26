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
		// 自动生成形状的定时任务
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

	// 创建矩形
	ShapeActorPtr CreateRectangle()
	{
		RectActorPtr rect = RectActor::Create(Size(40, 40));
		return rect;
	}

	// 创建椭圆形
	ShapeActorPtr CreateEllipse()
	{
		EllipseActorPtr ellipse = EllipseActor::Create(Vec2(30, 20));
		return ellipse;
	}

	// 创建多边形
	ShapeActorPtr CreatePolygon()
	{
		// 计算六边形的六个顶点
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

	// 创建三角形
	ShapeActorPtr CreateTriangle()
	{
		// 等边三角形边长
		float side = 45.f;

		// 使用形状生成器创建三角形三条边
		ShapeMakerPtr maker = ShapeMaker::Create();
		maker->BeginPath(Point(0, 0));
		maker->AddLine(Point(side, 0));
		maker->AddLine(Point(side / 2, side * math::Cos(30.0f)));
		maker->EndPath(true);

		// 获取生成的形状
		ShapePtr shape = maker->GetShape();
		// 创建形状角色
		ShapeActorPtr triangle = ShapeActor::Create(shape);
		return triangle;
	}

	// 将形状添加到场景中
	void AddShape(ShapeActorPtr shape, const Point& position)
	{
		// 设置形状填充颜色
		shape->SetFillColor(Color(Color::Blue, 0.5f));
		// 设置形状轮廓颜色
		shape->SetStrokeColor(Color(Color::Blue, 0.8f));
		// 加宽形状轮廓宽度
		shape->SetStrokeStyle(StrokeStyle::Create(1.3f));

		// 添加动画
		shape->AddAction(Tween::MoveTo(4_sec, Point(position.x, -50)).RemoveTargetWhenDone());
		shape->AddAction(Tween::RotateBy(1_sec, math::Random(40.0f, 120.0f)).SetLoops(-1));

		shape->SetRotation(math::Random(0.0f, 360.0f));
		shape->SetPosition(position);
		shape->SetAnchor(0.5f, 0.5f);
		this->AddChild(shape);
	}
};
