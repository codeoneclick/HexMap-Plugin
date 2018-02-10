// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M3Utilities.h"
#include "M3Entity.h"
#include "M3KVListener.h"

class HEXMAP_API M3Model_INTERFACE : public M3KVListener_INTERFACE {
protected:

	static std::set<uintptr_t> GuidsContainer;

public:

	M3Model_INTERFACE() = default;
	virtual ~M3Model_INTERFACE() = default;

	CTTI_CLASS_GUID(M3Model_INTERFACE, M3Model_INTERFACE::GuidsContainer)
};

template<typename T>
class M3Model : public M3Model_INTERFACE
{
static_assert(std::is_base_of<M3Entity, T>::value, "T must derive from M3Entity");
private:

	PROP_STATIC(public, M3Model<T>, TempContainer, std::shared_ptr<std::vector<std::shared_ptr<M3Model<T>>>>)

protected:

	std::array<std::shared_ptr<M3Model_INTERFACE>, std::numeric_limits<uint8_t>::max()> Submodels;

public:

	CTTI_CLASS_GUID(M3Model<T>, M3Model<T>::GuidsContainer)

	PROP_STRONG(public, M3Model<T>, Entity, std::shared_ptr<T>, nullptr)

	PROP_STATIC(public, M3Model<T>, Container, std::shared_ptr<std::vector<std::shared_ptr<M3Model<T>>>>)

	PROP_WEAK(public, M3Model<T>, Parent, std::weak_ptr<M3Model_INTERFACE>)

	M3Model() {
		Entity->Set(std::make_shared<T>());
		Submodels.fill(nullptr);
	};
	virtual ~M3Model() = default;

	virtual void Init() { };

	static void Register() {
		auto Storage = std::make_shared<std::vector<std::shared_ptr<M3Model<T>>>>();
		Container = std::make_shared<M3KVProperty<decltype(Storage)>>(Storage);
		Storage = std::make_shared<std::vector<std::shared_ptr<M3Model<T>>>>();
		TempContainer = std::make_shared<M3KVProperty<decltype(Storage)>>(Storage);
	};

	static void ApplyContainer() {
		Container->Set(TempContainer->Get());
	};

	void AddToContainer() {
		const auto Instance = M3Model_INTERFACE::shared_from_this();
		TempContainer->Get()->push_back(std::static_pointer_cast<M3Model<T>>(Instance));
	};

	template<typename TSubmodel>
	void AddSubmodel(const std::shared_ptr<TSubmodel>& Submodel) {
		static_assert(std::is_base_of<M3Model_INTERFACE, TSubmodel>::value, "TSubmodel must derive from M3Model_INTERFACE");
		if (!Submodels[Submodel->InstanceGuid()]) {
			Submodels[Submodel->InstanceGuid()] = Submodel;
			const auto Instance = M3Model_INTERFACE::shared_from_this();
			Submodel->Parent->Set(std::static_pointer_cast<M3Model<T>>(Instance));
		} else {
			assert(false);
		}
	};

	template<typename TSubmodel>
	void RemoveSubmodel(const std::shared_ptr<TSubmodel>& Submodel) {
		static_assert(std::is_base_of<M3Model_INTERFACE, TSubmodel>::value, "TSubmodel must derive from M3Model_INTERFACE");
		Submodels[Submodel->InstanceGuid()] = nullptr;
		Submodel->Parent->Set(nullptr);
	};

	template<typename TSubmodel>
	std::shared_ptr<TSubmodel> GetSubmodel() const {
		static_assert(std::is_base_of<M3Model_INTERFACE, TSubmodel>::value, "TSubmodel must derive from M3Model_INTERFACE");
		return Submodels[TSubmodel::ClassGuid()];
	};

	template<typename TModel>
	std::shared_ptr<TModel> GetParent() {
		static_assert(std::is_base_of<M3Model_INTERFACE, TModel>::value, "TModel must derive from M3Model_INTERFACE");
		std::shared_ptr<TModel> Result = nullptr;
		if (!Parent->Get().expired()) {
			Result = std::static_pointer_cast<TModel>(Parent->Get().lock());
		}
		return Result;
	};

	void Subscribe(const M3KVProperty_INTERFACE_SharedPtr& Prop, const std::string& PropId) {
		const auto Instance = M3KVListener_INTERFACE::shared_from_this();
		Prop->SetId(PropId);
		Prop->Subscribe(Instance);
	};

	void OnChanged(const M3KVProperty_INTERFACE_SharedPtr& Prop) {
		std::cout << Prop->GetId() << " Changed!" << std::endl;
	};
};

template<typename T>
PROP_STATIC_INIT(M3Model<T>, Container, std::shared_ptr<std::vector<std::shared_ptr<M3Model<T>>>>, nullptr)

template<typename T>
PROP_STATIC_INIT(M3Model<T>, TempContainer, std::shared_ptr<std::vector<std::shared_ptr<M3Model<T>>>>, nullptr)

#define SUBSCRIBE_PROP(__class__, __prop__) \
Subscribe(Entity->Get()->__prop__, __class__::PROPERTY_ID_##__prop__()); \

