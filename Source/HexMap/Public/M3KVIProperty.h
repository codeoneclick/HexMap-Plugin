// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"

class M3KVIProperty : public std::enable_shared_from_this<M3KVIProperty>
{
protected:

	std::string Id;
	bool bIsTrigger;

public:

	M3KVIProperty(bool bIsTrigger = false);
	~M3KVIProperty();

	std::string GetId() const;
};

M3KVIProperty::M3KVIProperty(bool bIsTrigger) {
	Id = "Unknonw";
	this->bIsTrigger = bIsTrigger;
}

M3KVIProperty::~M3KVIProperty() {

}

std::string M3KVIProperty::GetId() const {
	return Id;
}

