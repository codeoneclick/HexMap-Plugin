// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

template<typename T>
class M3KVProperty;

template<typename T>
class HEXMAP_API M3KVObserver
{
public:

	virtual void OnValueChanged(const M3KVProperty<T>& Property, T OldValue, T NewValue) = 0;
};
