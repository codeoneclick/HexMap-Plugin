// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <cassert>
#include <memory>
#include <functional>
#include <vector>
#include <algorithm>
#include "CoreMinimal.h"
#include "M3KVBinding.h"

template<typename T>
class M3KVConnection;

template<typename T>
class M3KVDispatcher : public M3KVBinding<T> 
{
private:

	friend class M3KVConnection<T>;

protected:

	std::vector<std::shared_ptr<M3KVConnection<T>>> Observers;

public:

	virtual ~M3KVDispatcher() = default;

	void Subscribe(const std::shared_ptr<M3KVConnection<T>>& Connection) {
		Observers.push_back(Connection);
	}

	void Unsubscribe(const std::shared_ptr<M3KVConnection<T>>& Connection) {
		std::remove_if(Observers.begin(), Observers.end(), [Connection](const std::shared_ptr<M3KVConnection<T>>& Observer) {
			return Observer == Connection;
		});
	}

	void UnsubscribeAll() {
		Observers.clear();
	}
};
