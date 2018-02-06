// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include <functional>
#include "CoreMinimal.h"

template<typename T>
class M3KVConnection;

template<typename T>
class M3KVBinding
{
public:
	virtual void Subscribe(const std::shared_ptr<M3KVConnection<T>>& Callback) = 0;
	virtual void Unsubscribe(const std::shared_ptr<M3KVConnection<T>>& Callback) = 0;
	virtual void UnsubscribeAll() = 0;
};