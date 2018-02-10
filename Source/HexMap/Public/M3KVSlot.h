// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3KVProperty.h"
#include "M3KVConnection.h"
#include "M3Utilities.h"

class M3KVSlot_INTERFACE : public std::enable_shared_from_this<M3KVSlot_INTERFACE>
{
public:

	virtual ~M3KVSlot_INTERFACE() = default;
};

template <typename T>
class M3KVSlot : public M3KVSlot_INTERFACE
{
private:

	std::shared_ptr<M3KVProperty<T>> Property;
	std::vector<std::shared_ptr<M3KVConnection<T>>> Connections;

public:

	M3KVSlot(const std::shared_ptr<M3KVProperty<T>> Prop) {
		Property = Prop;
	}

	std::shared_ptr<M3KVConnection<T>> Attach(const std::function<void(const T&)>& Callback) {
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

	void DetachAll() {
		for (const auto& Connection : Connections) {
			Property->Unsubscribe(Connection);
		}
		Connections.clear();
	}
};
