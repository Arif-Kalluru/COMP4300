#pragma once

#include "Entity.h"
#include <map>
#include <memory>
#include <vector>

typedef std::vector<std::shared_ptr<Entity> > entityVec;
typedef std::map<std::string, entityVec> entityMap;

class EntityManager {
	entityVec m_entities;	    // vector of all entities
	entityVec m_entitiesToAdd;  // queue of entities to be added
	entityMap m_entitiesMap;
	size_t m_totalEntities = 0;

	void removeDeadEntities(entityVec& vec);

public:
	EntityManager();

	void update();
	std::shared_ptr<Entity> addEntity(const std::string& tag);
	const entityVec& getEntities();
	const entityVec& getEntities(const std::string& tag);
};