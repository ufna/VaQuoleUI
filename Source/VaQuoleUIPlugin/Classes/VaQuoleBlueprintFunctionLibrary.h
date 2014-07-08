// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "VaQuoleUIPluginPrivatePCH.h"
#include "VaQuoleBlueprintFunctionLibrary.generated.h"

class UVaQuoleUIComponent;

/**
 * Class that handles view of one web page
 */
UCLASS(ClassGroup=UI)
class UVaQuoleBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/**
	 * Checks if the actor has WebUI component
	 *
	 * @param Actor			Actor to check the WebUI component
	 * @param WebUI			WebUI component if actor has it
	 * @return				True if actor has an WebUI component, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|VaQuoleUI|SceneUI", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static bool ActorHasWebUI(UObject* WorldContextObject, const AActor *Actor, UVaQuoleUIComponent*& WebUI);

};
