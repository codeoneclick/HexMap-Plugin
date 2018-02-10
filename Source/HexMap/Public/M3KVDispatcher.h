// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3KVBinding.h"
#include "M3Utilities.h"

class M3KVDispatcher : public M3KVBinding
{
protected:

	std::vector<M3KVListener_INTERFACE_SharedPtr> Listeners;

public:

	virtual ~M3KVDispatcher() = default;

	void Subscribe(const M3KVListener_INTERFACE_SharedPtr& Listener) {
		Listeners.push_back(Listener);
	}

	void Unsubscribe(const M3KVListener_INTERFACE_SharedPtr& Listener) {
		std::remove_if(Listeners.begin(), Listeners.end(), [Listener](const M3KVListener_INTERFACE_SharedPtr& aListener) {
			return aListener == Listener;
		});
	}

	void UnsubscribeAll() {
		Listeners.clear();
	}
};
