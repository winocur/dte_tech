#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL_opengl.h>

#include "agora.h"

WindowDimension windowDimension;
float gameTimer = 0;

#include "audio.cpp"
#include "sprites.cpp"
#include "input.cpp"
#include "text.cpp"
#include "grid.cpp"

#include "data.cpp"
#include "resources.h"
#include "buildings.cpp"
#include "level.cpp"
#include "simulation.cpp"
#include "ui.cpp"

#include "ECS/ECS.cpp"

void TestButton () {
    printf("Test button pressed!\n");
}

SDL_Surface* surface;


void GameInit(SDL_Surface * windowSurface, GameMemory * gameMemory) {

    //initialize game memory
    GameState * gs = (GameState *)gameMemory->permanentStorage;
    gs->camera.targetZoom = 1;
    gs->camera.actualZoom = 1;
    gameState = gs;
    
    surface = windowSurface;
    
    // create grid
    PopulateGrid(&gs->grid);
    grid = &gs->grid;

    // load fonts
    gs->mainFont = LoadFont("../assets/fonts/sansation/Sansation-Regular.ttf", 15);
    gs->titleFont = LoadFont ("../assets/fonts/sansation/Sansation-Regular.ttf", 20);
    
    gs->selectedBuildingId = -1;
    gs->session = StartGame();

    printf("Game state size: %i\n", sizeof(GameSession));
}

void CenterCamera (Building* building) {
    gameState->camera.targetTranslation = ScreenToProjection(GetScreenPosition (building, grid)) * -1 - Vector2 { 0, GetBuildingData(building->type)->height };
}

void GameUpdateAndRender (GameMemory * gm, WindowDimension windowDim, f64 msElapsed, GameInputFrame input) {
    
    windowDimension = windowDim;
    GLenum error = GL_NO_ERROR;

    float deltaTime = msElapsed / 1000.0;

    gameTimer += msElapsed;

    // input
    gameMemory = gm;
    MouseEvents mouseEvents = ProcessMouseInput(&input);
    GameState * gs = (GameState *)gameMemory->permanentStorage;
    gameState = gs;

    Camera* camera = &gs->camera;

    camera->targetZoom = Clamp(camera->targetZoom + mouseEvents.zoomChange, 0.5, 2.0);

    Vector2 worldMousePosition = mouseEvents.mousePosition;
    worldMousePosition = worldMousePosition - Vector2 { windowDimension.width / 2, windowDimension.height / 2 };
    worldMousePosition = worldMousePosition * camera->actualZoom;
    worldMousePosition = worldMousePosition - camera->actualTranslation;

    error = glGetError();
    if(error != GL_NO_ERROR) {
        printf("OpenGL Error: %s\n", gluErrorString( error ));
    }

    // clears temporary memory
    // We're doing this here instead of the frame boundary so buttons
    // created in the last frame can be interacted at the beggining of this
    // one creating a one-frame retained mode of sorts
    // that gets thrown away every frame
    gameMemory->temporaryStorageCurrent = gameMemory->temporaryStorage;

    // allocate space for up to 64 buttons to be filled by the menu rendering.
    gameState->buttons = GetFromTemporaryMemory<UIButton>(gameMemory, 64);
    gameState->buttonCounter = 0;
    
    // camera interp
    camera->actualZoom = Lerp(camera->actualZoom, camera->targetZoom, 10 * deltaTime);
    camera->actualTranslation = Lerp(camera->actualTranslation, camera->targetTranslation, 8 * deltaTime);

    // --  rendering --
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Pop default matrix onto current matrix
    glMatrixMode( GL_MODELVIEW );

    //Save default matrix again with camera translation
    glPushMatrix();

    // world space projection
    glOrtho( -windowDimension.width * camera->actualZoom / 2, windowDimension.width * camera->actualZoom / 2, -windowDimension.height * camera->actualZoom / 2, windowDimension.height * camera->actualZoom / 2, 1.0, -1.0 );
    glTranslatef(camera->actualTranslation.x,camera->actualTranslation.y, 0);  
      
    Tile hover = QueryTile (&gs->grid, worldMousePosition);
    HighlightTile(hover, { 0, 120, 120, 120 });

    RenderGrid (&gs->grid);

    glPopMatrix();
    //EOF
}

void GameCleanup(GameMemory * gm) {
    free (gm->permanentStorage);
    free (gm->temporaryStorage);
}

Vector2 WorldToScreenPosition (Vector2 worldPosition) {
    Camera camera = gameState->camera;
    Vector2 projectedPosition = ScreenToProjection(worldPosition);
    return Vector2 { projectedPosition.x / camera.actualZoom, projectedPosition.y / camera.actualZoom };
}

void ProcessEventsIdle (MouseEvents mouseEvents, Camera* camera, Vector2 worldMousePosition) {

    GameState* gs = gameState;

    if(mouseEvents.event == CLIC) {
        if(!OnClicUI(mouseEvents.mousePosition, gs->buttons, gs->buttonCounter)) {

            Tile selected = QueryTile (&gs->grid, worldMousePosition);
            gs->grid.selectedX = selected.indexX;
            gs->grid.selectedY = selected.indexY;

            // select a building
            Building* touched = QueryBuilding (&selected, gs->session.buildings, gs->session.buildingCounter);

            // unselect previous building
            if(gs->selectedBuildingId != -1) {
                Building* selectedBuilding = QueryBuildingById (gs->session.buildings, gs->session.buildingCounter, gs->selectedBuildingId);
                selectedBuilding->state = IDLE;
            }

            if(touched != NULL) {
                printf("Touched building: %s [%i,%i]\n", GetBuildingData(touched->type)->name, touched->gridX, touched->gridY);
                touched->state = SELECTED;
                CenterCamera(touched);
                gs->selectedBuildingId = touched->instanceId;
            } else {
                gs->selectedBuildingId = -1;
            }
        }
    } else if (mouseEvents.event == LEFT_CLIC) {
        OutputDebug("Left Clic");    
    } else if(mouseEvents.event == DRAG_CAMERA) {
        camera->targetTranslation = camera->targetTranslation + mouseEvents.mouseDelta * camera->actualZoom;
    } else if (mouseEvents.event == DRAG_ITEM) {

        Tile selected = QueryTile (&gs->grid, worldMousePosition);

        //gs->selectedBuilding->gridX = selected.indexX;
        //gs->selectedBuilding->gridY = selected.indexY;
    } else {
        OnHoverUI(mouseEvents.mousePosition, gs->buttons, gs->buttonCounter);
    }
}

Sprite* LoadSingleSprite (const char* fileName) {
    char path [128] = "../assets/images/";
    strcat(path, fileName);
    auto sheet = LoadSpriteSheet (&gameState->spriteSheets[gameState->nextSpriteSheet++], path, GL_RGBA, PNG, surface, 1,1);
    auto sprite = LoadSprite(&gameState->_sprites[gameState->nextSprite++],sheet, fileName, 0, 0);

    printf("Loaded %s\n", fileName);

    return sprite;
}

template <class T>
T* GetFromTemporaryMemory(GameMemory* gameMemory, int count) {

    T* data = (T*)gameMemory->temporaryStorageCurrent;
    gameMemory->temporaryStorageCurrent += sizeof(T*) * count;
    return data;
}