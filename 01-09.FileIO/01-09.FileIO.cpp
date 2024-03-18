﻿#include <iostream>
#include <fstream>
#include "ThirdParty/inicpp.h"
#include <format>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

int main()
{
	// 1. 기본 C++ file I/O
	// 2. ini 파일 I/O
	// 3. json 파일 I/O

	// 절대경로 : 특정 드라이브를 기준으로 (C:\...) 시작하는 경로 (C:\Dir\Test.txt)
	// 상대경로 : 특정 기준점으로부터 시작하는 경로 "Dir\Test.txt" "Dir/Test.txt"

	// 1. 기본 C++ file I/O(Input / Output)
	{
		// file write
		// output file stream : ofstream
		{
			std::ofstream OutputStream = std::ofstream("Test.txt"/*, std::ios::out*/);
			if (OutputStream.is_open())
			{
				std::string String = "Hello File!\n";
				OutputStream.write(String.data(), String.size());
				// OutputStream << String << std::endl;

				//int Int = 10;
				//OutputStream.write(reinterpret_cast<char*>(&Int), sizeof(Int));

				// OutputStream.close(); // 파일 닫기
				// 스코프를 벗어나면서 소멸자에서 알아서 close 호출
			}
		}

		{
			// file load
			std::ifstream InputStream("Test.txt"/*, std::ios::in*/);
			if (InputStream.is_open())
			{
				std::string String;
				String.resize(12);
				InputStream.read(String.data(), String.size()); // Hello File!\n
				// InputStream >> String; // Hello

				//int Int = 0;
				//InputStream.read(reinterpret_cast<char*>(&Int), sizeof(Int)); // 10

				InputStream.close(); // 파일 닫기
			}
		}

		// 1. 기본 C++ file IO binary
		{
			{
				std::ofstream OutputStream = std::ofstream("TestBinary.txt", std::ios::out | std::ios::binary);
				if (OutputStream.is_open())
				{
					std::string String = "Hello File!\n";
					OutputStream.write(String.data(), String.size());
					// OutputStream << String << std::endl;

					int Int = 10;
					OutputStream.write(reinterpret_cast<char*>(&Int), sizeof(Int));

					// OutputStream.close(); // 파일 닫기
					// 스코프를 벗어나면서 소멸자에서 알아서 close 호출
				}
			}
			{
				// file load
				std::ifstream InputStream("Test.txt", std::ios::in | std::ios::binary);
				if (InputStream.is_open())
				{
					std::string String;
					String.resize(12);
					InputStream.read(String.data(), String.size()); // Hello File!\n
					// InputStream >> String; // Hello

					int Int = 0;
					InputStream.read(reinterpret_cast<char*>(&Int), sizeof(Int)); // 10

					InputStream.close(); // 파일 닫기
				}
			}
		}
	}

	// 2. ini file I/O
	{
		// write
		{
			ini::IniFile File;
			File["Section"]["Key"] = "Value";
			File["Section"]["Key2"] = 1000;

			File["Graphics"]["Width"] = 1920;
			File["Graphics"]["Height"] = 1080;

			File.save("IniFile.ini");
		}
		// load
		{
			ini::IniFile File("IniFile.ini");
			std::string Value = File["Section"]["Key"].as<std::string>();
			int Value2 = File["Section"]["Key2"].as<int>();

			int Width = File["Graphics"]["Width"].as<int>();
			int Height = File["Graphics"]["Height"].as<int>();

			ini::IniSection Section = File["Graphics"];
			for (auto It : Section)
			{
				std::cout << std::format("Key : {}, Value : {}\n", It.first, It.second.as<std::string>());
			}
		}
	}

	// 3. json file IO
	{
		struct FPlayer
		{
			FPlayer() = default;
			FPlayer(std::string_view InName,const unsigned int InLevel)
				:Name(InName), Level(InLevel) { }
			~FPlayer() = default;

			void Save(rapidjson::Value& InOutValue, rapidjson::Document::AllocatorType& InAllocator)
			{
				InOutValue.AddMember("Name", rapidjson::StringRef(Name.data()), InAllocator);
				InOutValue.AddMember("Level", Level, InAllocator);
				InOutValue.AddMember("Exp", Exp, InAllocator);
				/*InOutValue["Name"] = rapidjson::StringRef(Name.data());
				InOutValue["Level"] = Level;
				InOutValue["Exp"] = Exp;*/ // allocator 할당이 안돼서 random access는 불가
			}

			void Load()
			{
			}

		private:
			std::string Name;
			unsigned int Level = 0;
			int Exp = 0;
		};

		using uint = unsigned int;
		constexpr uint PlayerNumbers = 20;
		// write
		{
			std::vector<FPlayer> Players;
			Players.reserve(PlayerNumbers);
			for (uint i = 0; i < PlayerNumbers; ++i)
			{
				// std::move : rvalue로 던짐
				// std::forward : 일반적으로 template에 사용, rvalue면 rvalue로 던지고, lvalue면 lvalue로 던짐 
				Players.emplace_back("Name" + std::to_string(i), i);
			}

			rapidjson::Document Doc(rapidjson::kObjectType);
			rapidjson::Document::AllocatorType& Allocator = Doc.GetAllocator();

			rapidjson::Value Array(rapidjson::kArrayType);
			for (FPlayer& It : Players)
			{
				rapidjson::Value Player(rapidjson::kObjectType);
				It.Save(Player, Allocator);
				Array.PushBack(Player, Allocator);
			}

			Doc.AddMember("PlayerInfo", Array, Allocator);

			rapidjson::StringBuffer Buffer;
			rapidjson::Writer<rapidjson::StringBuffer> Writer(Buffer);
			Doc.Accept(Writer);
			std::string Json(Buffer.GetString(), Buffer.GetSize());
			
			std::ofstream File("TestJson.json");
			File << Json;
		}
	}
}