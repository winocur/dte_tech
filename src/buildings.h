#ifndef H_BUILDINGS
#define H_BUILDINGS

enum BuildingState {
    IDLE,
    SELECTED,
    DESTROYED,
    MOVING,
};

enum BuildingType {
    
    BT_NONE = -1,

    // Housing
    BUILDING_BLOCKS = 0,
    SUBURBAN_HOUSES,
    COMMUNAL_HOUSING,
    TECHNO_KAUS,

    // Energy
    COAL_PLANT,
    NATURAL_GAS,
    NUCLEAR_PLANT,
    SOLAR_AND_WIND,

    // Production
    INDUSTRIAL_REFINERY,
    LOCAL_FACTORIES,
    ARTISAN_GUILDS,
    MONDRAGON_COLLECTIVE,

    // Exchange
    BUSINESS_OFFICES,
    SUPERMARKET,
    REGIONAL_BAZAAR,
    POTLACH_GATHERING,

    BT_NUM,
};

struct BuildingData {
    char* name; // 8
    
    int toxicity; // 4
    int housing;  // 4

    Resources io; // 6
    BuildingType type; // 1
    BuildingType evolution; // 1
    
    int clumpArea; // 4
    int size; // 4

    Color color; // 4
    int height; // 4

    char* audioEventName; // 8
};

struct Building {
    i16 gridX;
    i16 gridY;

    BuildingState state;
    BuildingType type;

    u16 instanceId;
};

void RenderBuilding (const Building* building, const TileGrid* grid);

void RenderBuildingClumping (const Building* building);

Building* QueryBuilding (const Tile* tile, Building* buildings, const u32 buildingCount);

Building* QueryBuildingById (Building* buildings, int buildingCount, int id);

void SortBuildings (const Building* buildings, Building* sortedBuildings, u32 buildingCounter);

Vector2 GetScreenPosition (const Building* building, const TileGrid* grid);

Building CreateBuilding (BuildingType type, int gridX, int gridY);

Building* FilterLiveBuildings (int* outCount, Building* originBuildings, const int originCount, GameMemory* memory); 

void RenderBuildingCollision (TileGrid* grid, Building* building);

bool ValidateBuildingPosition (Building* movingBuilding);

float GetBuildingToxicity (const Building* building);

#endif