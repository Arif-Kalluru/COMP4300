#pragma once

#include "Entity.hpp"

#include <map>
#include <memory>
#include <vector>

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;     // map from entity tag to vectors

class EntityManager {
private:
	EntityVec m_entities;	       // vector of all entities
	EntityVec m_entitiesToAdd;     // queue of entities to be added during next update
	EntityMap m_entitiesMap;       // map from entity tag to vectors
	uint64_t  m_totalEntities = 0; // total entities created

	void removeDeadEntities(EntityVec& vec);

public:
	EntityManager();

	const EntityVec& getEntities();
	const EntityVec& getEntities(const std::string& tag);

	std::shared_ptr<Entity> addEntity(const std::string& tag);
	void                    update();
};