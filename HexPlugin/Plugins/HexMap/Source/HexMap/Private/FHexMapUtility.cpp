// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "FHexMapUtility.h"

const FHexMapUtility::Orientation FHexMapUtility::LayoutPointy = Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5);
const FHexMapUtility::Orientation FHexMapUtility::LayoutFlat = Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0);

FHexMapUtility::FHexMapUtility()
{
}

FHexMapUtility::~FHexMapUtility()
{
}
