#include "simulation.h"

GameSession StartGame() {

    GameSession session;
    return session;
}

void ProcessTick (GameSession* session) {
    session->timer = 0;

}

float GetToxicity (const GameSession* session) {

    float toxicity = 0;

    for (int i = 0; i < session->buildingCounter; i++) {

        Building building = session->buildings[i];
        toxicity += GetBuildingToxicity(&building);
    }

    return toxicity;

}

Resources GetResourceIO (const GameSession* session) {

    Resources resources = Resources {0, 0, 0};

    for (int i = 0; i < session->buildingCounter; i++) {

        Building building = session->buildings[i];

        if(building.state != DESTROYED) {
            resources = resources + GetBuildingData(building.type)->io;
        }
    }

    return resources;
}

Resources GetUpgradeCost (const Building* building) {
    return Resources { 200, 100, 100 };
}

Resources GetDemolisionCost (const Building* building) {
    return Resources { 150, 20, 20 };
}

Resources GetMoveCost (const Building* building) {
    return Resources { 80, 50, 50 };
}

bool StartMovingBuilding (Building* building) {

    GameSession* session = &gameState->session;

    Resources cost = GetMoveCost(building);
    if(HasEnoughResources(session->resources, cost)) {
        session->resources = session->resources - cost;
        session->phase = GP_MOVING_BUILDING;
        building->state = MOVING;
    }
}

bool DemolishBuilding (Building* building) {

    GameSession* session = &gameState->session;

    Resources cost = GetDemolisionCost(building);
    if(HasEnoughResources(session->resources, cost)) {
        session->resources = session->resources - cost;
        building->state = DESTROYED;
        return true;
    }

    return false;
}

bool UpgradeBuilding (Building* building) {

    GameSession* session = &gameState->session;

    Resources cost = GetUpgradeCost(building);
    if(HasEnoughResources(session->resources, cost)) {
        session->resources = session->resources - cost;

        BuildingData* buildingData = GetBuildingData(building->type);

        if(buildingData->evolution == BT_NONE) {
            printf("Building has no evolution");
            return false;
        }

        BuildingType type = buildingData->evolution;
        building->type = type;
        return true;
    }

    return false;
}

Building* PlaceBuilding (Building building) {

    GameSession* session = &gameState->session;

    for (int i = 0; i < session->buildingCounter; i++) {
        if (session->buildings[i].state == DESTROYED) {
            session->buildings[i] = building;
            return session->buildings + i;
        }
    }

    session->buildings[session->buildingCounter] = building;
    session->buildingCounter++;
    return session->buildings + (session->buildingCounter - 1);
}
