// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"
#include <kiwano-audio/kiwano-audio.h>

class AudioDemo
	: public Stage
{
	RefPtr<audio::SoundPlayer> sound_player;	// 音乐播放器
	RefPtr<TextActor> volume_text;				// 音量文字
	RefPtr<TextActor> state_text;				// 播放状态文字
	bool is_playing = true;

public:
	static RefPtr<Stage> Create()
	{
		return new AudioDemo;
	}

	static String DemoName()
	{
		return "Audio Demo";
	}

	AudioDemo()
	{
		// 加载音乐
		sound_player = new audio::SoundPlayer;

		// 预加载音频，可以防止播放时卡顿
		sound_player->Preload("res/sounds/splash.mp3");

		// 播放音乐（参数用来设置播放循环次数，-1 表示循环播放）
		sound_player->Play("res/sounds/splash.mp3", -1);

		// 创建说明文字
		RefPtr<TextActor> intro = new TextActor("按上下键调整音量\n按空格键暂停或继续");
		intro->SetFillColor(Color::White);
		intro->SetAlignment(TextAlign::Center);
		intro->SetAnchor(0.5f, 0.5f);
		intro->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50);

		// 创建音量文字
		volume_text = new TextActor("当前音量：");
		volume_text->SetFillColor(Color::White);
		volume_text->SetAnchor(0.5f, 0.5f);
		volume_text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 30);

		// 创建状态文字
		state_text = new TextActor("当前状态：");
		state_text->SetFillColor(Color::White);
		state_text->SetAnchor(0.5f, 0.5f);
		state_text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 60);

		// 添加到舞台
		this->AddChild(intro);
		this->AddChild(volume_text);
		this->AddChild(state_text);
	}

	void OnUpdate(Duration dt) override
	{
		if (sound_player == nullptr)
			return;

		// 获取音量和播放状态
		float volume = sound_player->GetVolume();

		// 修改文本
		volume_text->SetText("当前音量：" + std::to_string(volume));
		state_text->SetText(is_playing ? "当前状态：正在播放" : "当前状态：停止播放");

		// 获取输入设备
		auto& input = Input::GetInstance();

		// 按空格键暂停或继续
		if (input.WasPressed(KeyCode::Space))
		{
			is_playing ? sound_player->PauseAll() : sound_player->ResumeAll();
			is_playing = !is_playing;
		}

		// 按上下键调整音量
		if (input.WasPressed(KeyCode::Up))
		{
			sound_player->SetVolume(volume + 0.1f);
		}
		else if (input.WasPressed(KeyCode::Down))
		{
			sound_player->SetVolume(volume - 0.1f);
		}
	}
};
