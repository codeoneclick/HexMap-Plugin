// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"

FORWARD_DECL_STRONG(M3KVListener_INTERFACE)

class M3KVBinding
{
public:
	virtual void Subscribe(const M3KVListener_INTERFACE_SharedPtr& Listener) = 0;
	virtual void Unsubscribe(const M3KVListener_INTERFACE_SharedPtr& Listener) = 0;
	virtual void UnsubscribeAll() = 0;
};