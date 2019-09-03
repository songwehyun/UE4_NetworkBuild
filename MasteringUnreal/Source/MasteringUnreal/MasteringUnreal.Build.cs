// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MasteringUnreal : ModuleRules
{
	public MasteringUnreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI

        /* 메뉴화면을 UMG로 제작할 것이므로 주석을 없앤다. */
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });

        /*온라인서브시스템과 인터페이스들은 일반적인 온라인 기능들을
         다양한 플랫폼(PS4,XBOX,STEAM등)에 정확한 추상적 개념들을 제공하기위해 존재하고 있다.
         온라인 서브시스템의 일반적인 목적은 플랫폼에 따라 코드들을 재사용할수 있게
         만들어 놓기 위함이다.
         온라인 서브시스템이 구현해놓은 일반적인 기능들은 다음과 같다.
         User Profiles, FriendsLists, Session Management,
         CloudServices,Leaderboards,Voice Chat,Achievements
         온라인서브시스템을 사용하기 위해서 모듈을 추가하는것 외에,
         [OnlineSubsystem]
         DefaultPlatformService=Null을 GameEngine.ini에 추가,
         IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
         온라인서브시스템 싱글톤을 호출하여야된다.
         DefaultPlatformService=Null은 온라인서브시스템을 null플랫폼에
         사용할수 있도록 활성화 하게 될것이다.
         null플랫폼이란 세션 호스트나 조인을 네트워크 디바이스에 다이렉트로 
         연결 할 수 있는 기능(로컬(LAN) 혹은 Public IP를 통하여)을 제공하는 일반적인 인터페이스이다.*/

        // Uncomment if you are using online features
        PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
