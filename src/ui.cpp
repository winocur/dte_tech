
#include "ui.h"

const float barWidthFactor = 0.3;
const float toxicityBarWidthFactor = 4.0;

void DrawWorldSpaceUI (GameSession* session, TTF_Font* mainFont, GameMemory* memory, const int selectedBuildingId, float cameraZoom) {
    
    Building* selectedBuilding = QueryBuildingById(session->buildings, session->buildingCounter, selectedBuildingId);

    if(selectedBuilding != NULL && session->phase == GP_IDLE) {
        DrawBuildingMenu (selectedBuilding, GetScreenPosition (selectedBuilding, grid), mainFont);
    }

}

void DrawBuildingMenu (Building* selectedBuilding, Vector2 buildingWorldPosition, TTF_Font* mainFont) {

    Vector2 buildingScreenPosition = WorldToScreenPosition (buildingWorldPosition) + Vector2 { 0, GetBuildingData(selectedBuilding->type)->height + 30 };

    int panelWidth = 180;

    BoundingBox panelBBox = { buildingScreenPosition.x, buildingScreenPosition.y, panelWidth, 230 };
    DrawPanel(panelBBox, { 0, 0, 0, 200 });

    // interaction blocker
    RegisterButton(UIButton { "Panel", panelBBox, [] () {}, NULL }, true);

    int accumulatedYPadding = 0;
    Text text;
    int length;
    
    // Building name
    char textBuffer [64];
    length = sprintf(textBuffer, "%s", GetBuildingData(selectedBuilding->type)->name);

    accumulatedYPadding += 30;
    CreateText(&text, textBuffer, mainFont);
    RenderText(&text, ParentBox (BoundingBox { 5, -accumulatedYPadding, 0, 30 }, panelBBox, BOTTOM_LEFT));
    
    UIAssets assets = gameState->uiAssets;

    accumulatedYPadding += 50;
    {
        auto toxicityBox = ParentBox(BoundingBox { 5, -accumulatedYPadding, 35, 35 }, panelBBox, BOTTOM_LEFT);
        DrawPanel(toxicityBox, Color { 40, 40, 40, 200 });
        RenderSpriteToBox (assets.toxicityIcon, toxicityBox);
        int barWidth = (int)(GetBuildingToxicity(selectedBuilding) * barWidthFactor * toxicityBarWidthFactor);
        auto barBox = ParentBox (BoundingBox { 40, -accumulatedYPadding + 8, barWidth, 19}, panelBBox, BOTTOM_LEFT);
        DrawPanel(barBox, ToxicColor);
    }

    accumulatedYPadding += 45;
    {
        int iconWidth  = 30;
        int iconHeight = 30;

        int halfWidth = iconWidth / 2;

        float power    = GetBuildingData(selectedBuilding->type)->io.power;
        float industry = GetBuildingData(selectedBuilding->type)->io.industry;
        float exchange = GetBuildingData(selectedBuilding->type)->io.exchange;
        
        Color bgColor = Color {80, 80, 80, 255 };

        auto buttonBox = ParentBox ({ panelWidth / 2 - halfWidth , -accumulatedYPadding, iconWidth, iconHeight }, panelBBox, BOTTOM_LEFT);
        DrawPanel(buttonBox, bgColor);
        RenderSpriteToBox (assets.powerIcon, buttonBox);

        // Draw Power Bar
        {
            int barWidth = power * barWidthFactor;
            int multiplier = (power > 0) ? 1 : -1;
            BoundingBox barBox = ParentBox ({ panelWidth / 2 + halfWidth * multiplier, -accumulatedYPadding + 7, barWidth, iconHeight - 14}, panelBBox, BOTTOM_LEFT);
            Color barColor = (power > 0) ? ResourceBarColor : Red;
            DrawPanel (barBox, barColor);
        }
        accumulatedYPadding += 34;

        buttonBox = ParentBox ({ panelWidth / 2 - halfWidth, -accumulatedYPadding, iconWidth, iconHeight }, panelBBox, BOTTOM_LEFT);
        DrawPanel(buttonBox, bgColor);
        RenderSpriteToBox (assets.industryIcon, buttonBox);
        
        // Draw Industry Bar
        {
            int barWidth = industry * barWidthFactor;
            int multiplier = (industry > 0) ? 1 : -1;
            BoundingBox barBox = ParentBox ({ panelWidth / 2 + halfWidth * multiplier, -accumulatedYPadding + 7, barWidth, iconHeight - 14}, panelBBox, BOTTOM_LEFT);
            Color barColor = (industry > 0) ? ResourceBarColor : Red;
            DrawPanel (barBox, barColor);
        }
        accumulatedYPadding += 34;

        buttonBox = ParentBox ({ panelWidth / 2 - halfWidth, -accumulatedYPadding, iconWidth, iconHeight }, panelBBox, BOTTOM_LEFT);
        DrawPanel(buttonBox, bgColor);
        RenderSpriteToBox (assets.exchangeIcon, buttonBox);

        // Draw Exchange Bar
        {
            int barWidth = exchange * barWidthFactor;
            int multiplier = (exchange > 0) ? 1 : -1;
            BoundingBox barBox = ParentBox ({ panelWidth / 2 + halfWidth * multiplier, -accumulatedYPadding + 7, barWidth, iconHeight - 14}, panelBBox, BOTTOM_LEFT);
            Color barColor = (exchange > 0) ? ResourceBarColor : Red;
            DrawPanel (barBox, barColor);
        }
        accumulatedYPadding += 60;

    }

    float accumulatedXPadding = 10;

    BoundingBox buttonBox = ParentBox (BoundingBox { accumulatedXPadding, -accumulatedYPadding, 40, 40 }, panelBBox, BOTTOM_LEFT);
    DrawPanel(buttonBox, {255,255,255,255});
    RenderSpriteToBox(assets.demolishIcon, buttonBox);
    RegisterButton(UIButton { "Dismantle", buttonBox, PressDemolish, GetDemolisionCost }, true);

    buttonBox.x += 50;
    DrawPanel(buttonBox, {255,255,255,255});
    RenderSpriteToBox(assets.upgradeIcon, buttonBox);
    RegisterButton (UIButton { "Upgrade", buttonBox, PressUpgrade, GetUpgradeCost }, true);

    buttonBox.x += 50;
    DrawPanel(buttonBox, {255,255,255,255});
    RenderSpriteToBox(assets.moveIcon, buttonBox);
    RegisterButton(UIButton { "Move", buttonBox, PressMove, GetMoveCost }, true);
}

void DrawMainPanel (GameSession* session, TTF_Font* mainFont, GameMemory* memory, float scale) {

    int accumulatedYPadding = 5;
    Text* text;
    int length;

    UIAssets assets = gameState->uiAssets;

    // background
    BoundingBox panelBBox = AnchorBox(BoundingBox {0, 0, 200, 200}, TOP_LEFT);
    DrawPanel(panelBBox, Color {0, 0, 0, 200 });
    RegisterButton(UIButton { "Panel", panelBBox, [] () {} }, false);

    // Population
    float populationWidthFactor = 0.005f;  
    {
        auto populationBox = ParentBox(BoundingBox { 5, accumulatedYPadding, 40, 40 }, panelBBox, TOP_LEFT);
        DrawPanel(populationBox, Color { 40, 40, 40, 200 });
        RenderSpriteToBox (assets.peopleIcon, populationBox);
        int barWidth = session->population * barWidthFactor * populationWidthFactor;
        auto barBox = ParentBox (BoundingBox { 45, accumulatedYPadding + 10, barWidth, 20}, panelBBox, TOP_LEFT);
        DrawPanel(barBox, Color { 125, 125, 125, 255});
    }
    
    accumulatedYPadding += 50;
    
    // Toxicity
    {
        auto toxicityBox = ParentBox(BoundingBox { 5, accumulatedYPadding, 40, 40 }, panelBBox, TOP_LEFT);
        DrawPanel(toxicityBox, Color { 40, 40, 40, 200 });
        RenderSpriteToBox (assets.toxicityIcon, toxicityBox);
        int barWidth = (int)(GetToxicity(session) * toxicityToPopulationDeaths * populationWidthFactor * barWidthFactor);
        auto barBox = ParentBox (BoundingBox { 45, accumulatedYPadding + 10, barWidth, 20}, panelBBox, TOP_LEFT);
        DrawPanel(barBox, ToxicColor);
    }

    accumulatedYPadding += 50;

    // resources header
    text = GetFromTemporaryMemory<Text>(memory);

    accumulatedYPadding += 30;
    CreateText(text, "RESOURCES", gameState->titleFont);
    RenderText(text, AnchorBox({ 0, 0, 200, accumulatedYPadding }, TOP_LEFT), scale);

    Color bgColor = Color {80, 80, 80, 255 };
    
    auto resourceBox = ParentBox(BoundingBox { 5, accumulatedYPadding, 40, 40 }, panelBBox, TOP_LEFT);
    
    auto resourceTurnProduction = GetResourceIO (session);

    const int stockBarHeight = 11;
    const int productionBarHeight = 7;

    // power
    {
        accumulatedYPadding += 10;
        DrawPanel(resourceBox, bgColor);
        RenderSpriteToBox (assets.powerIcon, resourceBox);
        int stockBarWidth = session->resources.power * barWidthFactor;
        auto stockBarBox = ParentBox (BoundingBox { 45, accumulatedYPadding, stockBarWidth, stockBarHeight}, panelBBox, TOP_LEFT);
        DrawPanel(stockBarBox, ResourceBarColor);

        accumulatedYPadding += 13;
        
        int productionBarWidth = Abs(resourceTurnProduction.power) * barWidthFactor;
        auto productionBarBox = ParentBox (BoundingBox { 45, accumulatedYPadding, productionBarWidth, productionBarHeight}, panelBBox, TOP_LEFT);
        DrawPanel(productionBarBox, (resourceTurnProduction.power > 0) ? ResourceProductionColor : Red);

        accumulatedYPadding += 30;
    }

    // industry
    {
        resourceBox = ParentBox(BoundingBox { 5, accumulatedYPadding, 40, 40 }, panelBBox, TOP_LEFT);
    
        DrawPanel(resourceBox, bgColor);
        RenderSpriteToBox (assets.industryIcon, resourceBox);

        accumulatedYPadding += 10;
        int barWidth = session->resources.industry * barWidthFactor;
        auto barBox = ParentBox (BoundingBox { 45, accumulatedYPadding, barWidth, stockBarHeight}, panelBBox, TOP_LEFT);
        DrawPanel(barBox, ResourceBarColor);

        accumulatedYPadding += 13;

        int productionBarWidth = Abs(resourceTurnProduction.industry) * barWidthFactor;
        auto productionBarBox = ParentBox (BoundingBox { 45, accumulatedYPadding, productionBarWidth, productionBarHeight }, panelBBox, TOP_LEFT);
        DrawPanel(productionBarBox, (resourceTurnProduction.industry > 0) ? ResourceProductionColor : Red);

        accumulatedYPadding += 30;
    }

    // exchange
    {
        resourceBox = ParentBox(BoundingBox { 5, accumulatedYPadding, 40, 40 }, panelBBox, TOP_LEFT);
    
        DrawPanel(resourceBox, bgColor);
        RenderSpriteToBox (assets.exchangeIcon, resourceBox);

        accumulatedYPadding += 10;
        int barWidth = session->resources.exchange * barWidthFactor;
        auto barBox = ParentBox (BoundingBox { 45, accumulatedYPadding, barWidth, stockBarHeight}, panelBBox, TOP_LEFT);
        DrawPanel(barBox, ResourceBarColor);

        accumulatedYPadding += 13;

        int productionBarWidth = Abs(resourceTurnProduction.exchange) * barWidthFactor;
        auto productionBarBox = ParentBox (BoundingBox { 45, accumulatedYPadding, productionBarWidth, productionBarHeight }, panelBBox, TOP_LEFT);
        DrawPanel(productionBarBox, (resourceTurnProduction.exchange > 0) ? ResourceProductionColor : Red);
    }
   
}

void DrawProjectedCosts (Resources costs, GameSession* session) {

    BoundingBox panelBBox = AnchorBox(BoundingBox {0, 0, 200, 200}, TOP_LEFT);
    int accumulatedYPadding = 135;
      
    auto resourceBox = ParentBox(BoundingBox { 5, accumulatedYPadding, 40, 40 }, panelBBox, TOP_LEFT);
    
    // ping pong alpha
    int alpha = ((int)(gameTimer * 0.5f)) % 510;
    alpha = (alpha > 255) ? 255 - (alpha - 255) : alpha;

    // power
    {
        auto barColor = (costs.power > session->resources.power) ? Color { 255, 0, 0, alpha } : Color { 255, 255, 0, alpha };
        
        int barWidth     = session->resources.power * barWidthFactor;
        int overlayWidth = Clamp(costs.power, 0, session->resources.power) * barWidthFactor;
        auto barBox      = ParentBox (BoundingBox { 45, accumulatedYPadding + 10, barWidth, 20}, panelBBox, TOP_LEFT);
        auto overlayBox  = ParentBox (BoundingBox { 0, 0, overlayWidth, 20 }, barBox, TOP_RIGHT);

        DrawPanel(overlayBox, barColor);

        accumulatedYPadding += 50;
    }

    // industry
    {
        auto barColor = (costs.industry > session->resources.industry) ? Color { 255, 0, 0, alpha } : Color { 255, 255, 0, alpha };
        
        int barWidth     = session->resources.industry * barWidthFactor;
        int overlayWidth = Clamp(costs.industry, 0, session->resources.industry) * barWidthFactor;
        auto barBox      = ParentBox (BoundingBox { 45, accumulatedYPadding + 10, barWidth, 20}, panelBBox, TOP_LEFT);
        auto overlayBox  = ParentBox (BoundingBox { 0, 0, overlayWidth, 20 }, barBox, TOP_RIGHT);

        DrawPanel(overlayBox, Color { 255, 255, 0, alpha });

        accumulatedYPadding += 50;
    }

    // exchange
    {
        auto barColor = (costs.exchange > session->resources.exchange) ? Color { 255, 0, 0, alpha } : Color { 255, 255, 0, alpha };
       
        int barWidth     = session->resources.exchange * barWidthFactor;
        int overlayWidth = Clamp(costs.exchange, 0, session->resources.exchange) * barWidthFactor;
        auto barBox      = ParentBox (BoundingBox { 45, accumulatedYPadding + 10, barWidth, 20}, panelBBox, TOP_LEFT);
        auto overlayBox  = ParentBox (BoundingBox { 0, 0, overlayWidth, 20 }, barBox, TOP_RIGHT);

        DrawPanel(overlayBox, Color { 255, 255, 0, alpha });
    }

}

void DrawBuildButton () {

    BoundingBox buttonBox = AnchorBox( { 10, 10, 100, 100 }, BOTTOM_RIGHT);
    DrawPanel(buttonBox, { 100, 100, 100, 100 } );
    RenderSpriteToBox(gameState->uiAssets.hammerIcon, buttonBox);
    RegisterButton(UIButton { "Build", buttonBox, [] () { gameState->session.phase = GP_BUILDING_MENU; }, NULL, [] () { return "Construct a new building"; } }, false);
}

void PressCreateBuilding (BuildingType type) {
    gameState->session.phase = GP_MOVING_BUILDING;
    Building building = CreateBuilding (type, 0, 0);
    building.state = MOVING;
    gameState->selectedBuildingId = PlaceBuilding(building)->instanceId;
}

void DrawBuildMenu () {

    DrawPanel(AnchorBox ( { 0, 0, 500, 300 }, CENTER), { 100, 100, 100, 255 } );
}

void DrawDebugUI (GameMemory* memory) {
    int accumulatedYPadding = -200;
    Text* text;
    int length;

    char textBuffer [64];
    length = sprintf(textBuffer, "Camera: %f, %f", gameState->camera.actualTranslation.x * -1, gameState->camera.actualTranslation.y * -1);

    text = GetFromTemporaryMemory<Text>(memory);

    CreateText(text, textBuffer, gameState->mainFont);
    RenderText(text, AnchorBox({ 0, 0, 200, accumulatedYPadding }, BOTTOM_LEFT), 1.0);
}

void DrawScreenSpaceUI (GameSession* session, TTF_Font* mainFont, GameMemory* memory) {
    
    DrawTickTimer (session);
    DrawMainPanel(session, mainFont, memory);
    DrawBuildButton();
    DrawCostContainer (session, mainFont, memory);
    DrawDebugUI(memory);

    if(session->phase == GP_BUILDING_MENU) {
        DrawBuildMenu();
    }
}

bool OnClicUI (Vector2 point, UIButton * buttons, int buttonCounter) {

    printf ("UI Clic at: %f, %f\n", point.x, point.y);

    for (int i = buttonCounter - 1; i >= 0; i--) {
        UIButton * button = buttons + i;
        if (CheckCollision(point, button->box)) {
            button->action();
            return true;
        }
    }

    return false;
}

bool OnHoverUI (Vector2 point, UIButton * buttons, int buttonCounter) {

    for (int i = buttonCounter - 1; i >= 0; i--) {
        UIButton * button = buttons + i;
        if (CheckCollision(point, button->box)) {
            gameState->hoverButton = button;
            return true;
        }
    }

    // we null the hover button to signal the UI not to draw cost panel
    gameState->hoverButton = NULL;
    return false;
}

BoundingBox ParentBox (BoundingBox box, BoundingBox parent, UIAnchor anchor) {

    auto transformedBox = AnchorBox (box, anchor);

    switch (anchor) {

        // @TODO revise, this are not made
        case TOP_LEFT      : return transformedBox + Vector2 { parent.x, - (windowDimension.height - parent.height - parent.y) };
        case TOP_RIGHT     : return transformedBox + Vector2 { - (windowDimension.width - parent.width - parent.x), - (windowDimension.height - parent.height - parent.y) };
        case CENTER        : return transformedBox + Vector2 { -parent.x, parent.y }; 
        case BOTTOM_CENTER : return transformedBox + Vector2 { -parent.x, parent.y };
        case BOTTOM_RIGHT  : return transformedBox + Vector2 { -parent.x, parent.y };


        case BOTTOM_LEFT   : return transformedBox + Vector2 { parent.x, parent.y + parent.height };
    }

    return transformedBox;
}

BoundingBox AnchorBox (BoundingBox box, UIAnchor anchor) {

    switch (anchor) {
        case TOP_LEFT : {
            return {
                box.x,
                windowDimension.height - box.y - box.height, 
                box.width,
                box.height,
            };
        }
        
        case TOP_RIGHT : {
            return {
                windowDimension.width - box.x - box.width,
                windowDimension.height - box.y - box.height, 
                box.width,
                box.height,
            };
        }

        case CENTER : {
            return {
                (windowDimension.width / 2) - (box.width / 2) + box.x,
                (windowDimension.height / 2) - (box.height / 2) + box.y, 
                box.width,
                box.height
            };
        }

        case BOTTOM_CENTER : {
            return {
                (windowDimension.width / 2) - (box.width / 2) + box.x,
                box.y, 
                box.width,
                box.height,
            };
        }

        case BOTTOM_RIGHT : {
            return {
                windowDimension.width - box.x - box.width,
                box.y, 
                box.width,
                box.height,
            };
        }

        case BOTTOM_LEFT : {
            return {
                box.x,
                box.y, 
                box.width,
                box.height,
            };
        }
    }

    return box;
}

void DrawPanel (BoundingBox box, Color color) {

    glColor4ub (color.r, color.g, color.b, color.a);

    glBegin(GL_QUADS);
        //top left
        glVertex3f( box.x, box.y, 0.5f);
        //top right
        glVertex3f( box.x + box.width, box.y, 0.5f);
        //bot right
        glVertex3f( box.x + box.width, box.y + box.height, 0.5f);
        //bot left
        glVertex3f( box.x, box.y + box.height, 0.5f);
    glEnd();

}

void RegisterButton (UIButton button, bool isWorldSpace) {

    // Transforms world
    if(isWorldSpace) {
        Vector2 screenPosition = WorldUIToScreenPosition( Vector2 { button.box.x, button.box.y });
        button.box = BoundingBox { screenPosition.x, screenPosition.y, button.box.width, button.box.height };
    }

    if(gameState->buttonCounter++ == 64) {
        printf("ERROR: no more than 64 buttons are allowed");
        return;
    }

    *(gameState->buttons + gameState->buttonCounter) = button;
    gameState->buttonCounter += 1;
}

Vector2 WorldUIToScreenPosition (Vector2 position) {
    Vector2 result = position + gameState->camera.actualTranslation;
    result = result * gameState->camera.actualZoom;
    result.x += windowDimension.width / 2;
    result.y += windowDimension.height / 2;
    return result;
}

void DrawCostContainer (GameSession* session, TTF_Font* mainFont, GameMemory* memory, float scale) {

    if (gameState->hoverButton == NULL) return;

    const UIButton* button = gameState->hoverButton;

    if(button->costs == NULL && button->hoverMessage == NULL) return;
    
    BoundingBox panelBBox = AnchorBox(BoundingBox {0,0,500,100} , BOTTOM_CENTER);
    
    Vector2 origin = { panelBBox.x, panelBBox.y + panelBBox.height };
    char textBuffer [64];

    if(button->costs != NULL) 
    {
        Building* selectedBuilding = QueryBuildingById(gameState->session.buildings, gameState->session.buildingCounter, gameState->selectedBuildingId);
        Resources costs = button->costs(selectedBuilding);
        DrawProjectedCosts(costs, session);
    }
    else
    {
        DrawPanel(panelBBox, {100,0,0,150});
        DoText(button->hoverMessage(), { origin.x + 20, origin.y - 20, 0, 20 }, mainFont);
    }
}

void DrawTickTimer (GameSession* session) {
    f64 percentaje = session->timer / session->tickTime;

    auto panelBox = AnchorBox({10,10,200,40 }, TOP_RIGHT);

    DrawPanel(panelBox, {200,200,200,230 });
    RegisterButton(UIButton { "TurnButton", panelBox, PressTurn, NULL }, false);
}

void PressDemolish () {
    printf("Demolish!\n");

    Building* selectedBuilding = QueryBuildingById(gameState->session.buildings, gameState->session.buildingCounter, gameState->selectedBuildingId);

    if(DemolishBuilding(selectedBuilding)) {
        gameState->selectedBuildingId = -1;
    }
}

void PressUpgrade () {
    printf("Upgrade!\n");

    Building* selectedBuilding = QueryBuildingById(gameState->session.buildings, gameState->session.buildingCounter, gameState->selectedBuildingId);

    if(UpgradeBuilding(selectedBuilding)) {
        gameState->selectedBuildingId = -1;
    }
}

void PressMove () {

    Building* selectedBuilding = QueryBuildingById(gameState->session.buildings, gameState->session.buildingCounter, gameState->selectedBuildingId);

    StartMovingBuilding(selectedBuilding);
}

void PressOpenBuildMenu () {
    gameState->session.phase = GP_BUILDING_MENU;
}

void PressTurn () {
    ProcessTick(&gameState->session);
}