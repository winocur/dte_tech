#include "ECS.h"
#include <cassert>

#include "component.h"
#include "component_gen.hpp"

void InitializeECS () {

}

Entity CreateEntity () {
    assert(livingEntityCount < MAX_ENTITIES && "Too many entities in existence");

    Entity entity = nextEntityId;
    nextEntityId++;
    livingEntityCount++;
}

void DestroyEntity (Entity entity) {
    signatures[entity] = 0x0;

    livingEntityCount--;
}

void SetSignature(Entity entity, Signature signature) {
	assert(entity < MAX_ENTITIES && "Entity out of range.");

	signatures[entity] = signature;
}

Signature GetSignature(Entity entity) {
	assert(entity < MAX_ENTITIES && "Entity out of range.");

	return signatures[entity];
}