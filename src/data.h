#ifndef H_DATA
#define H_DATA

BuildingData buildingData [32];

void LoadBuildingData ();

BuildingData* GetBuildingData(BuildingType type);

#endif