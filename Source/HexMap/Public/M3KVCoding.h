// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

template<typename T>
class M3KVCoding
{
public:
	virtual void Set(const T& Value) = 0;
	virtual T Get() = 0;
};
