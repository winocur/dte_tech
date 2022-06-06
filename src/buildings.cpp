#include "buildings.h"

void RenderBuilding (const Building* building, const TileGrid* grid) {

    Tile tile = grid->tiles[building->gridX][building->gridY];

    int roof = GetBuildingData(building->type)->height;
    int size = GetBuildingData(building->type)->size;

    BuildingData* buildingData = GetBuildingData(building->type);

    Vector2 bottomLeft = ScreenToProjection( { tile.center.x - grid->tileWidth / 2  - size * grid->tileWidth, tile.center.y - grid->tileHeight / 2 - size * grid->tileHeight }); 
    Vector2 topLeft    = ScreenToProjection( { tile.center.x - grid->tileWidth / 2     - size * grid->tileWidth, tile.center.y + grid->tileHeight / 2 + size * grid->tileHeight }); 
    Vector2 topRight   = ScreenToProjection( { tile.center.x + grid->tileWidth / 2    + size * grid->tileWidth, tile.center.y + grid->tileHeight / 2 + size * grid->tileHeight }); 
    Vector2 bottomRight = ScreenToProjection( { tile.center.x + grid->tileWidth / 2 + size * grid->tileWidth, tile.center.y - grid->tileHeight / 2 - size * grid->tileHeight });

    Color baseColor = buildingData->color;
    Color midColor  = baseColor + Color { - 50, -50, -50, 0};
    Color darkColor = baseColor + Color { - 100, -100, -100, 0};

    if(building->state == SELECTED) {

        glBegin (GL_QUADS);
        glColor4ub (255, 255, 125, 255);

        glVertex3f (bottomLeft.x, bottomLeft.y,  0);
        glVertex3f (topLeft.x, topLeft.y, 0);
        glVertex3f (topRight.x, topRight.y,  0);
        glVertex3f (bottomRight.x, bottomRight.y, 0);
        glVertex3f (bottomLeft.x, bottomLeft.y,  0);
        glEnd();

        RenderBuildingClumping(building);
    }

    glBegin (GL_QUADS);

    glColor4ub (darkColor.r, darkColor.g, darkColor.b, darkColor.a);

    //bottom face
    glVertex3f (bottomLeft.x, bottomLeft.y,  0);
    glVertex3f (topLeft.x, topLeft.y, 0);
    glVertex3f (topRight.x, topRight.y,  0);
    glVertex3f (bottomRight.x, bottomRight.y, 0);
    glVertex3f (bottomLeft.x, bottomLeft.y,  0);
    glEnd();
    
    //top face
    glBegin (GL_QUADS);

    glColor4ub (midColor.r, midColor.g, midColor.b, 230);

    glVertex3f (bottomLeft.x, bottomLeft.y + roof,  0);
    glVertex3f (topLeft.x, topLeft.y + roof, 0);
    glVertex3f (topRight.x, topRight.y + roof,  0);
    glVertex3f (bottomRight.x, bottomRight.y + roof, 0);
    glVertex3f (bottomLeft.x, bottomLeft.y + roof,  0);
    glEnd();
    
    //left wall
    glBegin (GL_QUADS);

    glColor4ub (baseColor.r, baseColor.g, baseColor.b, baseColor.a);

    glVertex3f (bottomLeft.x, bottomLeft.y,  0);
    glVertex3f (bottomLeft.x, bottomLeft.y + roof,  0);
    glVertex3f (bottomRight.x, bottomRight.y + roof, 0);
    glVertex3f (bottomRight.x, bottomRight.y, 0);
    glVertex3f (bottomLeft.x, bottomLeft.y,  0);
    
    glEnd();
    
    //right
    glBegin (GL_QUADS);
    glColor4ub (midColor.r, midColor.g, midColor.b, midColor.a);

    glVertex3f (topLeft.x, topLeft.y,  0);
    glVertex3f (topLeft.x, topLeft.y + roof,  0);
    glVertex3f (topRight.x, topRight.y + roof, 0);
    glVertex3f (topRight.x, topRight.y, 0);
    glVertex3f (topLeft.x, topLeft.y,  0);
    
    glEnd();

}

void RenderBuildingClumping (const Building* building) {

    int clumpingTiles = 0;
    BuildingData* buildingData = GetBuildingData(building->type);
    Tile* tiles = GetClumpingTiles(&gameState->grid, buildingData->clumpArea, building->gridX, building->gridY, buildingData->size, &clumpingTiles, gameMemory);

    for (int i = 0; i < clumpingTiles; i++) {
        HighlightTile(tiles[i], { 200, 200, 0, 150 });
    }

}

void RenderBuildingCollision (TileGrid* grid, Building* building) {

    int size = GetBuildingData(building->type)->size;
    for (int i = building->gridX - size; i <= building->gridX + size; i++)
    {
        for (int j = building->gridY - size; j <= building->gridY + size; j++) {

            Tile tile = grid->tiles[i] [j];
            Building* other = QueryBuilding(&tile, gameState->session.buildings, gameState->session.buildingCounter);

            if(other != NULL) {
                HighlightTile(tile, { 250, 0, 0, 250} );
            }
        }
    }
}

bool ValidateBuildingPosition (Building* building) {
    
    int size = GetBuildingData(building->type)->size;
    for (int i = building->gridX - size; i <= building->gridX + size; i++)
    {
        for (int j = building->gridY - size; j <= building->gridY + size; j++) {

            Tile tile = grid->tiles[i] [j];
            Building* other = QueryBuilding(&tile, gameState->session.buildings, gameState->session.buildingCounter);

            if(other != NULL) {
                return false;
            }
        }
    }

    return true;
}

Vector2 GetScreenPosition (const Building* building, const TileGrid* grid) {

    Tile tile = grid->tiles[building->gridX][building->gridY];
    return Vector2 { tile.center.x, tile.center.y };
}

Building* QueryBuilding (const Tile* tile, Building* buildings, const u32 buildingCount) {

    for (int i = 0; i < buildingCount; i++) {

        Building* building = buildings + i;

        if(building->state == DESTROYED || building->state == MOVING) continue;
        
        int size = GetBuildingData(building->type)->size;

        int relativeGridX = abs(building->gridX - tile->indexX);
        int relativeGridY = abs(building->gridY - tile->indexY);

        if(relativeGridX <= size  && relativeGridY <= size) return building;
    }

    return NULL;
}

Building* QueryBuildingById (Building* buildings, int buildingCount, int id) {
    
    if (id == -1) return NULL;

    for (int i = 0; i < buildingCount; i++) {
        if(buildings[i].instanceId == id) {
            return &buildings[i];
        }
    }

    printf ("ERROR: no building of id %i", id);
    return NULL;
}



float GetBuildingToxicity (const Building* building) {

    float clumpToxicity = 3.0;
    float baseToxicity = GetBuildingData(building->type)->toxicity;

    int clumpingTilesCounter;
    Tile* clumpingTiles = GetClumpingTiles (&gameState->grid, GetBuildingData(building->type)->clumpArea, building->gridX, building->gridY, GetBuildingData(building->type)->size, &clumpingTilesCounter, gameMemory);

    int clumpingBuildingCounter = 0;
    int* clumpingBuildingIds = (int*)gameMemory->temporaryStorageCurrent;

    for (int i = 0; i < clumpingTilesCounter; i++) {

        Building* nearbyBuilding = QueryBuilding(&clumpingTiles[i], gameState->session.buildings, gameState->session.buildingCounter);

        if(nearbyBuilding == NULL) continue;

        bool found = false;

        for (int j = 0; j < clumpingBuildingCounter; j++) {
            if(nearbyBuilding->instanceId == clumpingBuildingIds[j]) {
                found = true;
                break;
            }
        }  

        if(!found) {
            *(clumpingBuildingIds + clumpingBuildingCounter) = nearbyBuilding->instanceId;
            clumpingBuildingCounter ++;
            gameMemory->temporaryStorageCurrent += sizeof (int*);
        }

    } 

    return baseToxicity + (clumpingBuildingCounter * clumpToxicity);
}


// Sorts buildings for drawing. Writes on a copy buffer so as to
// not break all the goddam pointers to buildings
void SortBuildings (const Building* buildings, Building* sortedBuildings, u32 buildingCounter) {

    // copy
    for (int i = 0; i < buildingCounter; i++) {
        sortedBuildings[i] = buildings[i];
    }

    Building temp;

    // bubble sort
    for (int i = 0; i < buildingCounter; i++) {

        for (int j = 0; j < buildingCounter - i - 1; j++) {

            Building a = sortedBuildings [j];
            Building b = sortedBuildings [j+1];

            u8 aSize = GetBuildingData(a.type)->size;
            u8 bSize = GetBuildingData(b.type)->size;

            int yDifference = (a.gridY + aSize) - (b.gridY + bSize);
            int xDifference = (a.gridX - aSize) - (b.gridX - bSize);

            if(yDifference + xDifference > 0) {

                temp = b;
                sortedBuildings[j+1] = a;
                sortedBuildings[j] = temp;
            }
        }
    }
}

Building CreateBuilding (BuildingType type, int gridX, int gridY) {
    
    Building building;
    building.instanceId = gameState->nextBuildingInstanceId;
    gameState->nextBuildingInstanceId += 1;
    building.gridX = gridX;
    building.gridY = gridY;
    building.type = type;

    printf("Creating Building of type %i\n", type);
    
    return building;
}