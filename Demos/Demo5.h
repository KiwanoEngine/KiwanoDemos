// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"
#include <iostream>

class Demo5
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new Demo5;
	}

	Demo5()
	{
		// 添加按键监听
		AddListener<KeyDownEvent>(Closure(this, &Demo5::OnKeyDown));

		// 创建说明文字
		TextActorPtr intro = new TextActor(L"按G发送GET请求\n按P发送POST请求\n按U发送PUT请求\n按D发送DELETE请求");
		// 设置文字位置
		intro->SetAnchor(0.5f, 0.5f);
		intro->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		intro->SetFillColor(Color::White);

		// 添加到舞台
		this->AddChild(intro);
	}

	void OnEnter() override
	{
		// 进入舞台时打开控制台
		Logger::Instance().ShowConsole(true);
	}

	void OnExit() override
	{
		// 退出舞台时关闭控制台
		Logger::Instance().ShowConsole(false);
	}

	void OnKeyDown(Event* evt)
	{
		KGE_ASSERT(evt->IsType<KeyDownEvent>());

		// 按不同键发送不同请求
		auto key_evt = dynamic_cast<KeyDownEvent*>(evt);
		if (key_evt->code == KeyCode::G)
		{
			SendGetRequest();
		}
		else if (key_evt->code == KeyCode::P)
		{
			SendPostRequest();
		}
		else if (key_evt->code == KeyCode::U)
		{
			SendPutRequest();
		}
		else if (key_evt->code == KeyCode::D)
		{
			SendDeleteRequest();
		}
	}

	void SendGetRequest()
	{
		// 发送 GET 请求
		KGE_LOG(L"Start to send GET request...");

		HttpRequestPtr request = new HttpRequest;
		// 设置请求 URL
		request->SetUrl(L"http://httpbin.org/get");
		// 设置请求类型为 GET
		request->SetType(HttpRequest::Type::Get);
		// 设置请求完成后的回调函数
		request->SetResponseCallback(Closure(this, &Demo5::Complete));

		// 发送 HTTP 请求
		HttpClient::Instance().Send(request);
	}

	void SendPostRequest()
	{
		// 发送 POST 请求
		KGE_LOG(L"Start to send POST request...");

		// 创建 JSON 格式的 POST 数据
		Json request_data = {
			{ L"string",	L"test中文" },
			{ L"boolean",	true },
			{ L"integer",	12 },
			{ L"float",		3.125 },
			{ L"array",		{ 1, 2, 3, 4, 4.5 } },
			{ L"object",	{ L"key", L"value" } },
		};

		HttpRequestPtr request = new HttpRequest;
		request->SetUrl(L"http://httpbin.org/post");
		request->SetType(HttpRequest::Type::Post);
		// 设置 POST 请求的数据
		request->SetJsonData(request_data);
		request->SetResponseCallback(Closure(this, &Demo5::Complete));

		HttpClient::Instance().Send(request);
	}

	void SendPutRequest()
	{
		// 发送 PUT 请求
		KGE_LOG(L"Start to send PUT request...");

		// 创建 JSON 格式的 PUT 数据
		Json request_data = Json::array({ 1, 2, 3 });

		HttpRequestPtr request = new HttpRequest;
		request->SetUrl(L"http://httpbin.org/put");
		request->SetType(HttpRequest::Type::Put);
		// 设置 PUT 请求的数据
		request->SetJsonData(request_data);
		request->SetResponseCallback(Closure(this, &Demo5::Complete));

		HttpClient::Instance().Send(request);
	}

	void SendDeleteRequest()
	{
		// 发送 DELETE 请求
		KGE_LOG(L"Start to send DELETE request...");

		HttpRequestPtr request = new HttpRequest;
		request->SetUrl(L"http://httpbin.org/delete");
		request->SetType(HttpRequest::Type::Delete);
		request->SetResponseCallback(Closure(this, &Demo5::Complete));

		HttpClient::Instance().Send(request);
	}

	void Complete(HttpRequestPtr request, HttpResponsePtr response)
	{
		// 判断请求是否成功
		if (response->IsSucceed())
		{
			try
			{
				// 将获取到的数据解析成 JSON 格式
				Json response_data = Json::parse(response->GetData());
				Json result = {
					{L"HttpCode", response->GetResponseCode()},
					{L"Data", response_data},
				};

				KGE_LOG(L"Response:\n", result.dump(4));
			}
			catch (std::exception&)
			{
				KGE_ERROR(L"Parse JSON failed!");
			}
		}
		else
		{
			// 请求失败时打印错误信息
			OutputError(response);
		}
	}

	void OutputError(HttpResponsePtr response)
	{
		// 打印 HTTP 响应结果的状态码和错误信息
		Json result = {
				{L"HttpCode", response->GetResponseCode()},
				{L"Error", response->GetError()},
		};

		String result_str = result.dump(4);
		KGE_ERROR(L"Response: %s\n", result_str.c_str());
	}
};
