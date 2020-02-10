// Copyright (C) 2019 Nomango

#pragma once

#include <kiwano/kiwano.h>
#include <kiwano-imgui/kiwano-imgui.h>

using namespace kiwano;
using namespace kiwano::imgui;

KGE_DECLARE_SMART_PTR(ImGuiStage);
class ImGuiStage
	: public Stage
{
	SpritePtr logo;

public:
	ImGuiStage()
	{
		logo = Sprite::Create("logo.png");
		logo->SetSize(160, 160);
		logo->SetAnchor(0.5f, 0.5f);
		logo->SetPosition(GetWidth() / 2, GetHeight() / 2);
		AddChild(logo);

		// 创建 ImGui 图层
		ImGuiLayerPtr layer = ImGuiLayer::Create("ControlPanel", Closure(this, &ImGuiStage::ControlPanel));
		AddChild(layer);
	}

	void ControlPanel()
	{
		static bool visibility = true;
		static float rotation = 0.0f;
		static Color clear_color = Color(0xE5E5E5);

		ImGui::Begin("Control Panel");

		ImGui::Text("Use the following widgets to control actor behaviors");

		ImGui::Checkbox("visibility", &visibility);

		if (ImGui::Button(" < "))
		{
			logo->Move(-3.0f, 0.0f);
		}
		ImGui::SameLine();

		if (ImGui::Button(" ^ "))
		{
			logo->Move(0.0f, -3.0f);
		}
		ImGui::SameLine();

		if (ImGui::Button(" v "))
		{
			logo->Move(0.0f, 3.0f);
		}
		ImGui::SameLine();

		if (ImGui::Button(" > "))
		{
			logo->Move(3.0f, 0.0f);
		}

		ImGui::SliderFloat("rotation", &rotation, 0.0f, 360.0f);

		ImGui::ColorEdit3("clear color", (float*)&clear_color);

		if (ImGui::Button("reset"))
		{
			visibility = true;
			rotation = 0.0f;

			logo->SetPosition(GetWidth() / 2, GetHeight() / 2);
		}

		ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();

		// 修改可见性
		logo->SetVisible(visibility);

		// 修改旋转角度
		logo->SetRotation(rotation);

		// 修改窗口背景色
		Renderer::GetInstance().SetClearColor(clear_color);
	}
};
