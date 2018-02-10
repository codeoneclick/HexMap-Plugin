// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3KVDispatcher.h"
#include "M3KVCoding.h"
#include "M3Utilities.h"

class M3KVProperty_INTERFACE : public M3KVDispatcher, public std::enable_shared_from_this<M3KVProperty_INTERFACE>
{
protected:

	std::string Id;

public:

	M3KVProperty_INTERFACE() {
		Id = "Unknown";
	};

	virtual ~M3KVProperty_INTERFACE() = default;

	std::string GetId() const {
		return Id;
	};

	void SetId(const std::string& Id) {
		this->Id = Id;
	};
};
FORWARD_DECL_STRONG(M3KVProperty_INTERFACE)

template<typename T>
class M3KVProperty : public M3KVCoding<T>, public M3KVProperty_INTERFACE
{
private:

	T Value;

protected:

	void Init() {
		Getter = [this]() {
			return this->Value;
		};

		Setter = [this](const T& Value) {
			this->Value = Value;
			if (Listeners.size() != 0) {
				for (const auto& Listener : Listeners) {
					Listener->OnChanged(shared_from_this());
				}
			}
		};
	};

public:

	M3KVProperty() {
		Init();
	};

	M3KVProperty(const T& Value) {
		this->Value = Value;
		Init();
	}

	virtual ~M3KVProperty() = default;

	M3KVProperty(const M3KVProperty&) = delete;
	M3KVProperty& operator= (const M3KVProperty&) = delete;

	void Set(const T& Value) {
		if (!Setter)
		{
			assert(false);
		}
		Setter(Value);
	}

	T Get() const {
		if (!Getter)
		{
			assert(false);
		}
		return Getter();
	}
};