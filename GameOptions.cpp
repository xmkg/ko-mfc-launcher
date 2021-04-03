/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */


#include "stdafx.h"
#include "GameOptions.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#pragma region Definition

	std::vector<std::string> GameOptions::Server::IP;
	int GameOptions::Server::Version;
	bool GameOptions::Texture::EnableLevelOfDetail_Character;
	bool GameOptions::Texture::EnableLevelOfDetail_Shape;
	bool GameOptions::Texture::EnableLevelOfDetail_Terrain;
	bool GameOptions::Shadow::Enable;
	int GameOptions::Window::Width;
	int GameOptions::Window::Height;
	int GameOptions::Window::ColorDepth;
	int GameOptions::Window::ViewDistance;
	bool GameOptions::Window::Cursor;
	bool GameOptions::Window::WindowedMode;
	int GameOptions::Window::BuffLine;
	bool GameOptions::Window::EnableMovies;
	int GameOptions::Sound::Distance;
	bool GameOptions::Sound::EnableBackgroundMusic;
	bool GameOptions::Sound::EnableEffectMusic;
	bool GameOptions::Sound::EnableDuplicateMusic;
	bool GameOptions::Sound::EnableTalk;
	int GameOptions::Effect::MaximumEffectRenderCount;
	int GameOptions::Effect::MaximumParticleLoadCount;
	bool GameOptions::Effect::WeaponEffectsVisible;
	bool GameOptions::Pet::ShowBalloon;
	bool GameOptions::Pet::EnableAutoLoot;
	int  GameOptions::Pet::LootCoinThreshold;
	int GameOptions::Pet::BalloonAlpha;

#pragma endregion


GameOptions::GameOptions()
{
}


GameOptions::~GameOptions()
{
}

bool GameOptions::Read()
{
	try
	{
		boost::property_tree::ptree pt, pt2;
		boost::property_tree::ini_parser::read_ini("option.ini", pt);

		GameOptions::Texture::EnableLevelOfDetail_Character = pt.get<bool>("Texture.LOD_Chr", true);
		GameOptions::Texture::EnableLevelOfDetail_Shape = pt.get<bool>("Texture.LOD_Shape", true);
		GameOptions::Texture::EnableLevelOfDetail_Terrain = pt.get<bool>("Texture.LOD_Terrain", true);
		GameOptions::Shadow::Enable = pt.get<bool>("Shadow.Use", true);
		GameOptions::Window::Width = pt.get<int>("ViewPort.Width", 1024);
		GameOptions::Window::Height = pt.get<int>("ViewPort.Height", 768);
		GameOptions::Window::ColorDepth = pt.get<int>("ViewPort.ColorDepth", 32);
		GameOptions::Window::ViewDistance = pt.get<int>("ViewPort.Distance", 512);
		GameOptions::Window::WindowedMode = pt.get<bool>("Screen.WindowMode", true);
		GameOptions::Window::BuffLine = pt.get<int>("BufferLine.BuffLine", 20);
		GameOptions::Window::Cursor = pt.get<bool>("Cursor.WindowCursor", true);
		GameOptions::Sound::EnableBackgroundMusic = pt.get<bool>("Sound.Bgm", true);
		GameOptions::Sound::EnableDuplicateMusic = pt.get<bool>("Sound.Duplicate", true);
		GameOptions::Sound::EnableEffectMusic = pt.get<bool>("Sound.Effect", true);
		GameOptions::Sound::EnableTalk = pt.get<bool>("Sound.Talk", true);
		GameOptions::Sound::Distance = pt.get<int>("Sound.Distance", 48);
		GameOptions::Effect::MaximumEffectRenderCount = pt.get<int>("Effect.Count", 2000);
		GameOptions::Effect::MaximumParticleLoadCount = pt.get<int>("Effect.PartLoadCount", 10000);
		GameOptions::Effect::WeaponEffectsVisible = pt.get<bool>("WeaponEffect.EffectVisible", true);
		GameOptions::Window::EnableMovies = pt.get<bool>("Movie.MovieVisible", true);
		GameOptions::Pet::ShowBalloon = pt.get<bool>("PetOption.PetBalloon", true);
		GameOptions::Pet::LootCoinThreshold = pt.get<int>("PetOption.PetLootMoney", 1000);
		GameOptions::Pet::EnableAutoLoot = pt.get<bool>("PetOption.PetCountLoot", true);
		GameOptions::Pet::BalloonAlpha = pt.get<int>("PetOption.PetAlpha", 80);

		boost::property_tree::ini_parser::read_ini("server.ini", pt2);
		int scount = pt2.get<int>("Server.Count", 0);
		if (scount > 0)
		{
			char buf[255] = { 0 };
			for (int i = 0; i < scount; i++)
			{
				ZeroMemory(buf, 255);
				sprintf_s(buf, 255, "Server.IP%d", i);
				std::string server_ip = pt2.get<std::string>(buf, "127.0.0.1");
				GameOptions::Server::IP.push_back(server_ip);
			}
		}
		GameOptions::Server::Version = pt2.get<int>("Version.Files", 1534);
	}
	catch (...) {
		TRACE("Option.ini read fail");
	}

	
	
	//std::cout << pt.get<std::string>("Section1.Value1") << std::endl;
	//std::cout << pt.get<std::string>("Section1.Value2") << std::endl;
	return false;
}

bool GameOptions::SaveOptions()
{
	try
	{
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini("option.ini", pt);

		pt.put("Texture.LOD_Chr", static_cast<int>(GameOptions::Texture::EnableLevelOfDetail_Character));
		pt.put("Texture.LOD_Shape", static_cast<int>(GameOptions::Texture::EnableLevelOfDetail_Shape));
		pt.put("Texture.LOD_Terrain", static_cast<int>(GameOptions::Texture::EnableLevelOfDetail_Terrain));
		pt.put("Shadow.Use", static_cast<int>(GameOptions::Shadow::Enable));
		pt.put("ViewPort.Width", GameOptions::Window::Width);
		pt.put("ViewPort.Height", GameOptions::Window::Height);
		pt.put("ViewPort.ColorDepth", GameOptions::Window::ColorDepth);
		pt.put("ViewPort.Distance", GameOptions::Window::ViewDistance);
		pt.put("Screen.WindowMode", static_cast<int>(GameOptions::Window::WindowedMode));
		pt.put("BufferLine.BuffLine", GameOptions::Window::BuffLine);
		pt.put("Sound.Distance", GameOptions::Sound::Distance);
		pt.put("Sound.Bgm", static_cast<int>(GameOptions::Sound::EnableBackgroundMusic));
		pt.put("Sound.Duplicate", static_cast<int>(GameOptions::Sound::EnableDuplicateMusic));
		pt.put("Sound.Effect", static_cast<int>(GameOptions::Sound::EnableEffectMusic));
		pt.put("Sound.Talk", static_cast<int>(GameOptions::Sound::EnableTalk));
		pt.put("Effect.Count", GameOptions::Effect::MaximumEffectRenderCount);
		pt.put("Effect.PartLoadCount", GameOptions::Effect::MaximumParticleLoadCount);
		pt.put("WeaponEffect.EffectVisible", static_cast<int>(GameOptions::Effect::WeaponEffectsVisible));
		pt.put("Movie.MovieVisible", static_cast<int>(GameOptions::Window::EnableMovies));
		pt.put("PetOption.PetBalloon", static_cast<int>(GameOptions::Pet::ShowBalloon));
		pt.put("PetOption.PetLootMoney", GameOptions::Pet::LootCoinThreshold);
		pt.put("PetOption.PetCountLoot", static_cast<int>(GameOptions::Pet::EnableAutoLoot));
		pt.put("PetOption.PetAlpha", GameOptions::Pet::BalloonAlpha);
		boost::property_tree::ini_parser::write_ini("option.ini", pt);
		return true;

	}
	catch (...) { return false; }
	
	return true;
}

bool GameOptions::SaveServer()
{
	return false;
}
