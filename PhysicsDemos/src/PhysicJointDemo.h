// Copyright (C) 2019 Nomango

#pragma once
#include "CircleBody.h"

class Car;

class PhysicJointDemo
	: public Stage
{
public:
	static inline RefPtr<Stage> Create() { return new PhysicJointDemo; }

	static inline String Name() { return "Physic Joint Demo"; }

	static inline String Label() { return "按←→↓键控制小车"; }

	PhysicJointDemo();

	void OnUpdate(Duration dt) override;

private:
	RefPtr<Actor> map_;
	RefPtr<Car> car_;
};

// 地面
class Ground
	: public ShapeActor
{
public:
	Ground(RefPtr<physics::World> world, Point const& pos);

	// 生成地面路径点
	Vector<Point> GeneratePathPoints();
};

// 小车
class Car
	: public Actor
{
	RefPtr<ShapeActor> chassis_;
	b2WheelJoint* wheel_;

public:
	Car(RefPtr<physics::World> world, Point const& pos);

	// 给小车后轮提供动力
	void SetSpeed(float speed);

	// 获取小车躯干的位置
	Point GetPosition() const;
};


PhysicJointDemo::PhysicJointDemo()
{
	// 创建物理世界
	auto world = new physics::World(b2Vec2(0, 10.f));
	AddComponent(world);

	// 创建地图
	map_ = new Actor();
	AddChild(map_);

	// 创建地面
	RefPtr<Actor> ground = new Ground(world, Point(0, GetHeight() - 200));
	map_->AddChild(ground);

	// 创建小车
	car_ = new Car(world, Point(190, 240));
	map_->AddChild(car_);
}

void PhysicJointDemo::OnUpdate(Duration dt)
{
	// 获取小车位置
	Point pos = car_->GetPosition();
	if (pos.x > GetWidth() / 2)
	{
		// 小车位置超过屏幕一半大小时, 移动整个地图, 让视角跟随小车
		map_->SetPositionX(GetWidth() / 2 - pos.x);
	}
	else
	{
		map_->SetPositionX(0.f);
	}

	// 按键处理
	auto& input = Input::GetInstance();
	if (input.IsDown(KeyCode::Right))
	{
		car_->SetSpeed(360.0f * 3);
	}
	else if (input.IsDown(KeyCode::Left))
	{
		car_->SetSpeed(-360.0f * 3);
	}
	else if (input.IsDown(KeyCode::Down))
	{
		car_->SetSpeed(0);
	}
}

Ground::Ground(RefPtr<physics::World> world, Point const& pos)
{
	// 设置形状颜色和位置
	this->SetStrokeColor(Color::White);
	this->SetPosition(pos);

	// 生成路径点
	Vector<Point> path_points = this->GeneratePathPoints();

	// 根据路径点绘制路径
	ShapeMaker maker;
	maker.BeginPath(path_points[0]);
	for (size_t i = 1; i < path_points.size(); ++i)
	{
		maker.AddLine(path_points[i]);
	}
	maker.EndPath();
	this->SetShape(maker.GetShape());

	// 根据路径点生成物理边
	b2BodyDef def;
	def.type = b2_staticBody;
	auto body = world->AddBody(&def);
	for (size_t i = 1; i < path_points.size(); ++i)
	{
		b2Vec2 start = physics::LocalToWorld(path_points[i - 1]);
		b2Vec2 end = physics::LocalToWorld(path_points[i]);
		b2EdgeShape shape;
		shape.Set(start, end);

		b2FixtureDef fd;
		fd.density = 0.0f;
		fd.friction = 0.6f;
		fd.shape = &shape;
		body->GetB2Body()->CreateFixture(&fd);
	}
	this->AddComponent(body);
}

Vector<Point> Ground::GeneratePathPoints()
{
	Vector<Point> path_points;	// 路径点

	path_points.push_back(Point(5, -400));
	path_points.push_back(Point(5, 0));
	path_points.push_back(Point(200, 0));

	float hs[] = { -25, -100, -250, 0, 0, 100, 200, 200, 125, 0 };
	float x = 200, dx = 500;

	for (int i = 0; i < 10; ++i)
	{
		float y = hs[i];
		path_points.push_back(Point(x + dx, y));
		x += dx;
	}

	for (int i = 0; i < 10; ++i)
	{
		float y = hs[i];
		path_points.push_back(Point(x + dx, y));
		x += dx;
	}

	path_points.push_back(Point(x, -400));
	return path_points;
}

Car::Car(RefPtr<physics::World> world, Point const& pos)
	: wheel_(nullptr)
{
	// 小车躯干形状顶点
	Vector<Point> vertices = { Point(-150, 50), Point(150, 50), Point(150, 0), Point(0, -90), Point(-115, -90), Point(-150, -20), };

	// 生成小车形状
	ShapeMaker maker;
	maker.BeginPath(vertices[0]);
	maker.AddLines(vertices);
	maker.EndPath(true);

	// 创建小车躯干
	RefPtr<ShapeActor> chassis = new ShapeActor(maker.GetShape());
	chassis->SetPosition(pos + Point(0, -100));
	chassis->SetStrokeColor(Color::White);
	this->AddChild(chassis);
	this->chassis_ = chassis;

	// 创建小车躯干的物理身体
	b2BodyDef def;
	def.type = b2_dynamicBody;
	auto chassis_body = world->AddBody(&def);
	chassis->AddComponent(chassis_body);
	{
		const auto b2vertices = physics::LocalToWorld(vertices);
		b2PolygonShape shape;
		shape.Set(b2vertices.data(), int32(b2vertices.size()));

		b2FixtureDef fd;
		fd.density = 1.0f;
		fd.friction = 0.2f;
		fd.shape = &shape;
		chassis_body->GetB2Body()->CreateFixture(&fd);
	}

	// 创建左右轮子
	RefPtr<Circle> lwheel = new Circle(world, pos + Point(-100, -35), 40.0f);
	RefPtr<Circle> rwheel = new Circle(world, pos + Point(100, -40), 40.0f);
	this->AddChild(lwheel);
	this->AddChild(rwheel);

	// 设置摩擦力
	lwheel->body->GetB2Body()->GetFixtureList()->SetFriction(0.9f);
	rwheel->body->GetB2Body()->GetFixtureList()->SetFriction(0.9f);

	// 创建左轮子关节
	{
		b2WheelJointDef wheel_def;
		wheel_def.Initialize(chassis_body->GetB2Body(), lwheel->body->GetB2Body(), lwheel->body->GetB2Body()->GetPosition(), b2Vec2(0, 1));
		wheel_def.frequencyHz = 4.0f;		// 频率和阻尼率赋予关节弹性
		wheel_def.dampingRatio = 0.7f;
		wheel_def.enableMotor = true;		// 启用马达
		wheel_def.motorSpeed = 0.f;			// 初始马达速度为零
		wheel_def.maxMotorTorque = 20.f;	// 最大马达转矩
		this->wheel_ = (b2WheelJoint*)world->AddJoint(&wheel_def);
	}

	// 创建右轮子关节
	{
		b2WheelJointDef wheel_def;
		wheel_def.Initialize(chassis_body->GetB2Body(), rwheel->body->GetB2Body(), rwheel->body->GetB2Body()->GetPosition(), b2Vec2(0, 1));
		wheel_def.frequencyHz = 4.0f;
		wheel_def.dampingRatio = 0.7f;
		wheel_def.enableMotor = false;
		wheel_def.motorSpeed = 0.f;
		wheel_def.maxMotorTorque = 10.f;
		world->AddJoint(&wheel_def);
	}
}

void Car::SetSpeed(float speed)
{
	wheel_->SetMotorSpeed(math::Degree2Radian(speed));
}

Point Car::GetPosition() const
{
	return chassis_->GetPosition();
}
