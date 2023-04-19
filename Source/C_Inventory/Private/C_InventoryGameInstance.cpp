// Fill out your copyright notice in the Description page of Project Settings.


#include "C_InventoryGameInstance.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Intro/IntroHUD.h"




UC_InventoryGameInstance::UC_InventoryGameInstance()
{
}

UC_InventoryGameInstance::~UC_InventoryGameInstance()
{
	MyDestroySession();
}


void UC_InventoryGameInstance::Init()
{
	Super::Init();

	OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem) {
		//Session interface
		if (SessionInterface = OnlineSubsystem->GetSessionInterface()) {

			/*Bind Func*/
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UC_InventoryGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UC_InventoryGameInstance::OnDestroySessionComplete);

		}
		
	}


}

void UC_InventoryGameInstance::OnCreateSessionComplete(FName SessionnName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Session Create Complete : %d"), bWasSuccessful);

	if (bWasSuccessful) {
		/*Clear Bind*/
		SessionInterface->ClearOnCreateSessionCompleteDelegates(this);

		/*Server 전용 -> 모든 Client가 따라감*/
		GetWorld()->ServerTravel("/Game/Lobby/LobbyMap?listen");

	}

}

void UC_InventoryGameInstance::OnDestroySessionComplete(FName SessionnName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Session Destroy Complete : %d"), bWasSuccessful);

	if (bWasSuccessful) {
		/*Clear Bind*/
		SessionInterface->ClearOnDestroySessionCompleteDelegates(this);
	}

}

void UC_InventoryGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Session Find Complete : %d"), bWasSuccessful);

	if (bWasSuccessful) {

		SearchResults = SearchSession->SearchResults;

		/*Find Session 존재*/
		if (SearchResults.Num()) {

			if (APlayerController* Controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
				if (AIntroHUD* HUD = Cast<AIntroHUD>(Controller->GetHUD())) {
					HUD->FindSessionComplete(SearchResults);
				}
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Session Not Found"));
		}

		/*Clear Bind*/
		SessionInterface->ClearOnFindSessionsCompleteDelegates(this);


	}
	
}


void UC_InventoryGameInstance::MyCreateSession(FName NewSessionName, FName Password)
{
	if (SessionInterface.IsValid()) {

		MySessionName = NewSessionName;
		MyPassword = Password;

		/*Session Setting*/
		FOnlineSessionSettings SessionSettings;

		SessionSettings.bIsDedicated = false;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.NumPublicConnections = 10;
		SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() != "NULL" ? false : true;


		//~ Custom Session setting.
		AddExtraSetting(FName(TEXT("sessionname")), MySessionName.ToString(), &SessionSettings);
		AddExtraSetting(FName(TEXT("password")), MyPassword.ToString(), &SessionSettings);


		/*Create Session*/
		SessionInterface->CreateSession(0, MySessionName, SessionSettings);

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can Not Create Session : Not Valid Session Interface"));
	}
}

void UC_InventoryGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{

	/*Is Valid SessionInterface*/
	if (SessionInterface.IsValid())
	{
		FString ConnectionInfo = FString();
		SessionInterface->GetResolvedConnectString(SessionName, ConnectionInfo);
		if (!ConnectionInfo.IsEmpty())
		{
			if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				PlayerController->ClientTravel(ConnectionInfo, TRAVEL_Absolute);
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can Not Join Session : Not Valid Session Interface"));
	}

	if (SessionInterface.IsValid()) {
		SessionInterface->ClearOnJoinSessionCompleteDelegates(this);
	}
	

}

void UC_InventoryGameInstance::MyDestroySession()
{
	if (SessionInterface.IsValid()) {
		SessionInterface->DestroySession(MySessionName);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can Not Destroy Session : Not Valid Session Interface"));
	}
}

void UC_InventoryGameInstance::MyJoinSession(FSessionData NewSessionData)
{
	if (SessionInterface.IsValid()) {

		/*Bind Func*/
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UC_InventoryGameInstance::OnJoinSessionComplete);


		SessionInterface->JoinSession(0, FName(*NewSessionData.SessionName), SearchResults[NewSessionData.SessionIndex]);
	}

}


void UC_InventoryGameInstance::MyFindSession()
{
	if (SessionInterface.IsValid()) {

		/*Search Settings (Match Create Session Settings)*/
		SearchSession = MakeShareable(new FOnlineSessionSearch());

		 //Is LAN
		SearchSession->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() != "NULL" ? false : true;
		SearchSession->MaxSearchResults = 10000;
		SearchSession->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);


		/*bind Find Session Complete*/
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UC_InventoryGameInstance::OnFindSessionComplete);

		/*Search Session*/
		SessionInterface->FindSessions(0, SearchSession.ToSharedRef());

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can Not Find Session : Not Valid Session Interface"));
	}

}

void UC_InventoryGameInstance::AddExtraSetting(FName NewKey, FString NewValue, FOnlineSessionSettings* MySessionSettings)
{
	FName Key = NewKey;
	FString Value = NewValue;
	FOnlineSessionSetting Settings = FOnlineSessionSetting(Value, EOnlineDataAdvertisementType::ViaOnlineService);
	MySessionSettings->Settings.Add(Key, Settings);
}

