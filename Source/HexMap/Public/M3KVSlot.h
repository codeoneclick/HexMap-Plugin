// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include <functional>
#include <vector>
#include "CoreMinimal.h"
#include "M3KVProperty.h"
#include "M3KVConnection.h"

template <typename T>
class M3KVSlot
{
private:

	std::shared_ptr<M3KVProperty<T>> Property;
	std::vector<std::shared_ptr<M3KVConnection<T>>> Connections;

public:

	M3KVSlot(const std::shared_ptr<M3KVProperty<T>> Property) {
		this->Property = Property;
	}

	std::shared_ptr<M3KVConnection<T>> Attach(const std::function<void(const T&, const T&)>& Callback) {
		std::shared_ptr<M3KVConnection<T>> Connection = nullptr;
		if (Callback) {
			Connection = std::make_shared<M3KVConnection<T>>(Callback);
			Connections.push_back(Connection);
			Property->Subscribe(Connection);
		}
		return Connection;
	}

	void Detach(const std::shared_ptr<M3KVConnection<T>>& Connection) {
		std::remove_if(Connections.begin(), Connections.end(), [Connection](const std::shared_ptr<M3KVConnection<T>>& aConnection) {
			return aConnection == Connection;
		});
		Property->Unsubscribe(Connection);
	}
};
