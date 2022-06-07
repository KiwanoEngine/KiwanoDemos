// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"
#include <iostream>

class NetworkDemo
	: public Stage
{
	String text_;

public:
	static StagePtr Create()
	{
		return new NetworkDemo;
	}

	static String DemoName()
	{
		return "Network Demo";
	}

	NetworkDemo()
	{
		// ����GUI�������
		ImGuiLayerPtr control_panel = new ImGuiLayer("Control", Closure(this, &NetworkDemo::ControlPanel));
		this->AddChild(control_panel);
	}

	void ControlPanel()
	{
		// imgui��������
		ImGui::SetNextWindowPosCenter(ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(500, 430), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowFocus();

		// ��ʾimgui����
		auto flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;
		ImGui::Begin("Try to send HTTP requests", nullptr, flags);

		// ��ʾ�ĸ���ť��������HTTP����
		if (ImGui::Button("GET"))
		{
			SendGetRequest();
		}
		ImGui::SameLine();

		if (ImGui::Button("POST"))
		{
			SendPostRequest();
		}
		ImGui::SameLine();

		if (ImGui::Button("PUT"))
		{
			SendPutRequest();
		}
		ImGui::SameLine();

		if (ImGui::Button("DELETE"))
		{
			SendDeleteRequest();
		}

		// ��ʾ����Ľ��
		ImGui::Text("Output:");

		char* buf = (char*)text_.c_str();
		ImGui::InputTextMultiline("Output", buf, text_.size(), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 25), ImGuiInputTextFlags_ReadOnly);

		// ��ս������
		if (ImGui::Button("Clear output"))
		{
			text_.clear();
		}

		// imgui���ڽ���
		ImGui::End();
	}

	void SendGetRequest()
	{
		// ���� GET ����
		text_.append("Start to send GET request...\n");

		// ����HTTP����
		HttpRequestPtr request = new HttpRequest(
			"http://httpbin.org/get",
			HttpType::Get,
			Closure(this, &NetworkDemo::Complete)
		);

		// ���� HTTP ����
		HttpModule::GetInstance().Send(request);
	}

	void SendPostRequest()
	{
		// ���� POST ����
		text_.append("Start to send POST request...\n");

		// ���� JSON ��ʽ�� POST ����
		Json request_data = {
			{ "string", "a simple string" },
			{ "boolean", true },
			{ "integer", 12 },
			{ "float", 3.125 },
			{ "array", { 1, 2, 3, 4, 4.5 } },
			{ "object", {{ "key", "value" }, { "key2", "value2" }} },
		};

		HttpRequestPtr request = new HttpRequest(
			"http://httpbin.org/post",
			HttpType::Post,
			request_data,
			Closure(this, &NetworkDemo::Complete)
		);

		HttpModule::GetInstance().Send(request);
	}

	void SendPutRequest()
	{
		// ���� PUT ����
		text_.append("Start to send PUT request...\n");

		// ���� JSON ��ʽ�� PUT ����
		Json request_data = Json::array({ 1, 2, 3 });

		HttpRequestPtr request = new HttpRequest(
			"http://httpbin.org/put",
			HttpType::Put,
			request_data,
			Closure(this, &NetworkDemo::Complete)
		);

		HttpModule::GetInstance().Send(request);
	}

	void SendDeleteRequest()
	{
		// ���� DELETE ����
		text_.append("Start to send DELETE request...\n");

		HttpRequestPtr request = new HttpRequest(
			"http://httpbin.org/delete",
			HttpType::Delete,
			Closure(this, &NetworkDemo::Complete)
		);

		HttpModule::GetInstance().Send(request);
	}

	void Complete(HttpRequestPtr request, HttpResponsePtr response)
	{
		// �ж������Ƿ�ɹ�
		if (response->IsSucceed())
		{
			try
			{
				// ����ȡ�������ݽ����� JSON ��ʽ
				Json response_data = Json::parse(response->GetData());
				Json result = {
					{"HttpCode", response->GetResponseCode()},
					{"Data", response_data},
				};

				text_.append("Response:\n")
					.append(result.dump(4))
					.append("\n");
			}
			catch (std::exception&)
			{
				text_.append("Parse JSON failed!\n");
			}
		}
		else
		{
			// ����ʧ��ʱ��ӡ������Ϣ
			OutputError(response);
		}
	}

	void OutputError(HttpResponsePtr response)
	{
		// ��ӡ HTTP ��Ӧ�����״̬��ʹ�����Ϣ
		Json result = {
				{"HttpCode", response->GetResponseCode()},
				{"Error", response->GetError()},
		};

		text_.append("Response:\n")
			.append(result.dump(4))
			.append("\n");
	}
};
