// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3KVIProperty.h"

class M3KVIListener
{
public:
	M3KVIListener() = default;
	~M3KVIListener() = default;

	virtual void OnChanged(const std::shared_ptr<M3KVIProperty> Property) = 0;
};
