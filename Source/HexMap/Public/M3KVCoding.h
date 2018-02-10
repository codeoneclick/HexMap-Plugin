// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

template<typename T>
class M3KVCoding
{
protected:

	std::function<T()> Getter;
	std::function<void(const T&)> Setter;

public:

	M3KVCoding() {
		Getter = nullptr;
		Setter = nullptr;
	}

	virtual ~M3KVCoding() = default;

	virtual void Set(const T& Value) = 0;
	virtual T Get() const = 0;
};
