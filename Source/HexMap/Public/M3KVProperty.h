// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3KVDispatcher.h"
#include "M3KVCoding.h"

template<typename T>
class M3KVProperty : public M3KVDispatcher<T>, public M3KVCoding<T>
{
private:

	T Value;
	bool IsTrigger;

public:

	M3KVProperty(const T& Value, bool IsTrigger = false) {
		this->Value = Value;
		this->IsTrigger = IsTrigger;
	}

	virtual ~M3KVProperty() = default;

	void Set(const T& Value) {
		const auto OldValue = this->Value;
		this->Value = Value;
		if (Observers.size() != 0) {
			for (const auto& Observer : Observers) {
				Observer->Call(OldValue, Value);
			}
		}
	}

	T Get() {
		return Value;
	}
};
