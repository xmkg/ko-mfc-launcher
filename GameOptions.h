/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */


#pragma once
class GameOptions
{
public:
	GameOptions();
	~GameOptions();

	class Server
	{
	public:
		static std::vector<std::string> IP;
		static int Version;
	};

	static bool Read();
	static bool SaveOptions();
	static bool SaveServer();
	class Texture
	{
	public:
		static bool EnableLevelOfDetail_Character;
		static bool EnableLevelOfDetail_Shape;
		static bool EnableLevelOfDetail_Terrain;
	};
	class Shadow
	{
	public:
		static bool Enable;
	};
	class Window
	{
	public:
		static int Width;
		static int Height;
		static int ColorDepth;
		static int ViewDistance;
		static bool Cursor;
		static bool WindowedMode;
		static int BuffLine;
		static bool EnableMovies;
	};

	class Sound
	{
	public:
		static int Distance;
		static bool EnableBackgroundMusic;
		static bool EnableEffectMusic;
		static bool EnableDuplicateMusic;
		static bool EnableTalk;
	};

	class Effect
	{
	public:
		static int MaximumEffectRenderCount;
		static int MaximumParticleLoadCount;
		static bool WeaponEffectsVisible;
	};
	class Pet
	{
	public:
		static bool ShowBalloon;
		static bool EnableAutoLoot;
		static int  LootCoinThreshold;
		static int BalloonAlpha;
	};
};

