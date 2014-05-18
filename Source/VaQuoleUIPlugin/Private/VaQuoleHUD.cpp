// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaQuoleUIPluginPrivatePCH.h"

AVaQuoleHUD::AVaQuoleHUD(const class FPostConstructInitializeProperties& PCIP) 
	: Super(PCIP)
{
	VaQuoleUI = PCIP.CreateDefaultSubobject<UVaQuoleUIComponent>(this, TEXT("VaQuoleHUD"));
}

void AVaQuoleHUD::DrawHUD()
{
	Super::DrawHUD();

	// Check main HUD texture resolution
	if (VaQuoleUI->GetWidth() != Canvas->ClipX || VaQuoleUI->GetHeight() != Canvas->ClipY)
	{
		VaQuoleUI->Resize(Canvas->ClipX, Canvas->ClipY);
	}

	if (bShowHUD)
	{
		// Draw hud web view texture
		FCanvasTileItem TileItem(FVector2D(0, 0), VaQuoleUI->GetTexture()->Resource, FVector2D(Canvas->ClipX, Canvas->ClipY), FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}
