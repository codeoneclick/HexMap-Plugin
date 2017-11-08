// Fill out your copyright notice in the Description page of Project Settings.

#include "../HexMapPrivatePCH.h"

#if WITH_EDITOR

#include "HexMap.h"
#include "HexMapDetailCustomization.h"
#include "HexMapGrid.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"

TSharedRef<IDetailCustomization> HexMapDetailCustomization::MakeInstance()
{
	return MakeShareable(new HexMapDetailCustomization);
}

void HexMapDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& HexMapCategory = DetailBuilder.EditCategory("HexMap");
	TSharedRef<IPropertyHandle> PropertyHandle_Tile_BP = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AHexMapGrid, Tile_BP));
	HexMapCategory.AddProperty(PropertyHandle_Tile_BP);
}

#endif


