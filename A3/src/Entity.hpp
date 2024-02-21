#pragma once

#include "Components.hpp"

#include <string>
#include <tuple>

typedef std::tuple<
	CTransform,
	CInput,
	CLifespan,
	CAnimation,
	CState,
	CBoundingBox,
	CGravity
> ComponentTuple;

class Entity
{
private:
	friend class EntityManager;

	bool           m_active     = true;
	std::string    m_tag        = "default";
	size_t         m_id         = 0;
	ComponentTuple m_components;

	// Private constructor, EntityManager has friend access
	// Only EntityManager should be allowed to make new Entities
	Entity(const size_t id, const std::string& tag);

public:
	const std::string& tag()      const;
	bool               isActive() const;
	const size_t       id()       const;
	void               destroy();

	template <typename T>
	T& getComponent()
	{
		return std::get<T>(m_components);
	}

	template <typename T>
	const T& getComponent() const
	{
		return std::get<T>(m_components);
	}

	// Parameter packing. && means it's an r-value reference
	// Ex: entity->addComponent<CTransform>(Vec2(100, 100), Vec2(5, 5));
	// Ex: entity->addComponent<CInput>();
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template <typename T>
	bool hasComponent() const
	{
		return getComponent<T>().has;
	}

	template <typename T>
	void removeComponent()
	{
		auto& component = getComponent<T>();
		component = T();
		component.has = false;
	}
};