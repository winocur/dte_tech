#include "data.h"

BuildingData MakeBuildingData (BuildingType type) {
    
    BuildingData data;

    switch (type)
    {
        // HOUSING 0
        case BUILDING_BLOCKS: {
            data = BuildingData {
                "Building Blocks",
                20,  // toxicity
                500000, // housing
                Resources {
                    -40, // power
                    -50, // production
                    -40  // exchange
                },
                type,
                SUBURBAN_HOUSES, // evolution
                4,  //clumping
                2,  //size
                Color {100, 100, 100, 200},
                80, // height
                NULL,
            };
        } break;
        
        // HOUSING 1
        case SUBURBAN_HOUSES: {
            data = BuildingData {
                "Suburban Houses",
                12,  // toxicity
                200000, // housing
                Resources {
                    -32, // power
                    -42, // production
                    -32  // exchange
                },
                type,
                COMMUNAL_HOUSING, // evolution
                3,  //clumping
                2,  //size
                Color {80, 130, 100, 200},
                70, // height
                NULL,
            };
        } break;
        
        // HOUSING 2
        case COMMUNAL_HOUSING : {
            data = BuildingData {
                "Communal Housing",
                8,  // toxicity
                100000, // housing
                Resources {
                    -22, // power
                    -35, // production
                    -22  // exchange
                },
                type,
                TECHNO_KAUS, // evolution
                3,  //clumping
                2,  //size
                Color {80, 150, 100, 200},
                60, // height
                NULL,
            };
        } break;

        // HOUSING 3
        case TECHNO_KAUS : {
            data = BuildingData {
                "Techno-Kaus",
                3,  // toxicity
                500000, // housing
                Resources {
                    -18, // power
                    -25, // production
                    -18  // exchange
                },
                type,
                BT_NONE, //evolution
                3,  //clumping
                2,  //size
                Color {100, 200, 100, 220},
                50, // height
                NULL,
            };
        } break;

        // ENERGY 0
        case COAL_PLANT: {
            data = BuildingData {
                "Coal Power Plant",
                40,  // toxicity
                0, // housing
                Resources {
                    500, // power
                    -60, // production
                    -40  // exchange
                },
                type,
                NATURAL_GAS,
                3,  //clumping
                3, //size
                Color {130, 100, 100, 220},
                80, // height
                
                NULL,
            };
        } break;

        // ENERGY 1
        case NATURAL_GAS: {
            data = BuildingData {
                "Natural Gas Extractor",
                30,  // toxicity
                0, // housing
                Resources {
                    400, // power
                    -50, // production
                    -30  // exchange
                },
                type,
                NUCLEAR_PLANT,
                3,  //clumping
                3, //size
                Color {150, 100, 100, 220},
                70, // height
                NULL,
            };
        } break;

        // ENERGY 2
        case NUCLEAR_PLANT: {
            data = BuildingData {
                "Nuclear Fission Plant",
                20,  // toxicity
                0, // housing
                Resources {
                    300, // power
                    -40, // production
                    -25  // exchange
                },
                type, 
                SOLAR_AND_WIND,
                3,  //clumping
                3, //size
                Color {170, 100, 90, 220},
                60, // height
                NULL,
            };
        } break;

        // ENERGY 3
        case SOLAR_AND_WIND: {
            data = BuildingData {
                "Solar and Wind",
                8,  // toxicity
                0, // housing
                Resources {
                    150, // power
                    -20, // production
                    -12  // exchange
                },
                type,
                BT_NONE,
                3,  //clumping
                3, //size
                Color {130, 80, 130, 220},
                40, // height
                NULL,
            };
        } break;

        // PRODUCTION 0
        case INDUSTRIAL_REFINERY: {
            data = BuildingData {
                "Factories and Refineries",
                30,  // toxicity
                0, // housing
                Resources {
                    -80, // power
                    300, // production
                    -60  // exchange
                },
                type,
                BT_NONE,
                3,  //clumping
                3, //size
                Color { 110, 130, 110, 220},
                70, // height
                "Refinery",
            };
         } break;

        // EXCHANGE 1
        case BUSINESS_OFFICES : {
            data = BuildingData {
                "Business Towers",
                30,  // toxicity
                0, // housing
                Resources {
                    -70, // power
                    -40, // production
                    300  // exchange
                },
                type,
                BT_NONE,
                3,  //clumping
                2, // size
                Color { 150, 150, 150, 220},
                90, // height
                NULL,
            };
        } break;

        data.type = type;

        return data;
    }
}

void LoadBuildingData () {

    for (int i = 0; i < BT_NUM; i++) {

        buildingData [i] = MakeBuildingData((BuildingType)i);
    }
}

BuildingData* GetBuildingData(BuildingType type) {
    return buildingData + type;
}