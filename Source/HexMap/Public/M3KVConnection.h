// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>
#include "CoreMinimal.h"

template<typename T>
class M3KVConnection
{
private:

	std::function<void(const T&, const T&)> Callback;

public:
	M3KVConnection(const std::function<void(const T&, const T&)>& Callback) {
		this->Callback = Callback;
	}

	void Call(const T& ValueA, const T& ValueB) {
		return Callback(ValueA, ValueB);
	}
};
