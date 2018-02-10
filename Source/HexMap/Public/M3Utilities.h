// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <map>
#include <set>
#include <list>
#include <forward_list>
#include <queue>
#include <string>
#include <vector>
#include <array>
#include <bitset>
#include <numeric>
#include <future>
#include <algorithm>
#include <mutex>
#include <thread>
#include <fstream>
#include <strstream>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <functional>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctime>
#include <iomanip>

#define CTTI_CLASS_GUID(__class__, __guids_container__, ...) \
static uint8_t ClassGuid() \
{ \
static uint8_t Guid = 0; \
static std::once_flag CachedClassesGuids; \
std::call_once(CachedClassesGuids, [] { \
__guids_container__.insert(reinterpret_cast<uintptr_t>(&ClassGuid)); \
Guid = __guids_container__.size(); \
});\
return Guid;\
}\
virtual uint8_t InstanceGuid() __VA_ARGS__\
{\
return __class__::ClassGuid();\
}\

#define PROP_STRONG(__directive__, __class__, __prop__, __type__, ...) \
__directive__: \
std::shared_ptr<M3KVProperty<__type__>> __prop__ = std::make_shared<M3KVProperty<__type__>>(__VA_ARGS__); \
static std::string __class__::PROPERTY_ID_##__prop__() { \
return #__prop__; \
} \

#define PROP_WEAK(__directive__, __class__, __prop__, __type__, ...) \
__directive__: \
std::shared_ptr<M3KVProperty<__type__>> __prop__ = std::make_shared<M3KVProperty<__type__>>(__VA_ARGS__); \
static std::string __class__::PROPERTY_ID_##__prop__() { \
return #__prop__; \
} \

#define PROP_STATIC(__directive__, __class__, __prop__, __type__, ...) \
__directive__: \
static std::shared_ptr<M3KVProperty<__type__>> __prop__; \
static std::string __class__::PROPERTY_ID_##__prop__() { \
return #__prop__; \
} \

#define PROP_STATIC_INIT(__class__, __prop__, __type__, ...) \
std::shared_ptr<M3KVProperty<__type__>> __class__::__prop__ = std::make_shared<M3KVProperty<__type__>>(__VA_ARGS__); \

#define FORWARD_DECL_STRONG(__class__) \
class __class__; \
typedef std::shared_ptr<__class__> __class__##_SharedPtr; \

#define FORWARD_DECL_WEAK(__class__) \
class __class__; \
typedef std::weak_ptr<__class__> __class__##_WeakPtr; \

#define FORWARD_DECL_CONTAINER(__class__, __type__) \
class __class__; \
typedef std::shared_ptr<std::vector<std::shared_ptr<__type__>>> __class__##_Container; \

