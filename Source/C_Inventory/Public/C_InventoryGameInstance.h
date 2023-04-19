// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Structs.h"
#include "C_InventoryGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class C_INVENTORY_API UC_InventoryGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UC_InventoryGameInstance();

	~UC_InventoryGameInstance();

protected:
	class IOnlineSubsystem* OnlineSubsystem;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SearchSession;

	TArray<FOnlineSessionSearchResult> SearchResults;

	FName MySessionName = FName("Default Session Name");
	FName MyPassword = FName("");

protected:
	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName ServerName, bool bWasSuccessful);

	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	virtual void OnFindSessionComplete(bool bWasSuccessful);

	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);


public:

	UFUNCTION(BlueprintCallable)
	void MyCreateSession(FName NewSessionName, FName Password);

	UFUNCTION(BlueprintCallable)
	void MyDestroySession();

	UFUNCTION(BlueprintCallable)
	void MyJoinSession(FSessionData NewSessionData);

	UFUNCTION(BlueprintCallable)
	void MyFindSession();


private:

	void AddExtraSetting(FName NewKey, FString NewValue, FOnlineSessionSettings* MySessionSettings);
};
