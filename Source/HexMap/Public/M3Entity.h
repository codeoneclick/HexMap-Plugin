// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3KVProperty.h"

class HEXMAP_API M3Entity : public std::enable_shared_from_this<M3Entity>
{
private:

protected:

	static std::set<uintptr_t> GuidsContainer;

public:

	CTTI_CLASS_GUID(M3Entity, M3Entity::GuidsContainer)

	PROP_STRONG(public, M3Entity, IsAssignedToView, bool, false)

	M3Entity() = default;
	virtual ~M3Entity() = default;
};
