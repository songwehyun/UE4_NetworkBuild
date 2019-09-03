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

        /* �޴�ȭ���� UMG�� ������ ���̹Ƿ� �ּ��� ���ش�. */
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });

        /*�¶��μ���ý��۰� �������̽����� �Ϲ����� �¶��� ��ɵ���
         �پ��� �÷���(PS4,XBOX,STEAM��)�� ��Ȯ�� �߻��� ������� �����ϱ����� �����ϰ� �ִ�.
         �¶��� ����ý����� �Ϲ����� ������ �÷����� ���� �ڵ���� �����Ҽ� �ְ�
         ����� ���� �����̴�.
         �¶��� ����ý����� �����س��� �Ϲ����� ��ɵ��� ������ ����.
         User Profiles, FriendsLists, Session Management,
         CloudServices,Leaderboards,Voice Chat,Achievements
         �¶��μ���ý����� ����ϱ� ���ؼ� ����� �߰��ϴ°� �ܿ�,
         [OnlineSubsystem]
         DefaultPlatformService=Null�� GameEngine.ini�� �߰�,
         IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
         �¶��μ���ý��� �̱����� ȣ���Ͽ��ߵȴ�.
         DefaultPlatformService=Null�� �¶��μ���ý����� null�÷�����
         ����Ҽ� �ֵ��� Ȱ��ȭ �ϰ� �ɰ��̴�.
         null�÷����̶� ���� ȣ��Ʈ�� ������ ��Ʈ��ũ ����̽��� ���̷�Ʈ�� 
         ���� �� �� �ִ� ���(����(LAN) Ȥ�� Public IP�� ���Ͽ�)�� �����ϴ� �Ϲ����� �������̽��̴�.*/

        // Uncomment if you are using online features
        PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
