// Fill out your copyright notice in the Description page of Project Settings.


#include "BPFL_Steam.h"
#include "ThirdParty/Steamworks/Steamv153/sdk/public/steam/steam_api.h"

FString UBPFL_Steam::GetSteamIDByString()
{
	FString ReturnValue{};

	if (SteamAPI_Init()) {

		uint64 SteamID{ 0 };
		CSteamID InSteamID = SteamUser()->GetSteamID();
		SteamID = InSteamID.ConvertToUint64();

		if (SteamID != 0) {
			/*%llu == long long unsigned*/
			ReturnValue = FString::Printf(TEXT("%llu"), SteamID);
		}
		else {
			ReturnValue = "Bad Steam ID";
		}

	}
	else {
		ReturnValue = "No Connection To Steam Initialized";
	}
		 
	return ReturnValue;
}
