#include "Entity.hpp"
#include "EntityManager.hpp"

#include <algorithm>

EntityManager::EntityManager()
{
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	vec.erase(
		std::remove_if(
			vec.begin(),
			vec.end(),
			[](auto it) { return !it->isActive(); }
		),
		vec.end()
	);
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	// TODO: Handle error
	return m_entitiesMap[tag];
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	// Entities are added to queue. Entities are added during next update
	m_entitiesToAdd.push_back(entity);
	return entity;
}

void EntityManager::update()
{
	for (const auto e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entitiesMap[e->tag()].push_back(e);
	}

	// clear queue when all are added
	m_entitiesToAdd.clear();

	// remove dead entites from the vector of all entities
	removeDeadEntities(m_entities);

	// remove dead entities from each vector in the entity map
	for (auto& [tag, entityVec_] : m_entitiesMap)
	{
		removeDeadEntities(entityVec_);
	}
}