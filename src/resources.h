#ifndef H_RESOURCES
#define H_RESOURCES

const int MAX_RESOURCE = 700;

struct Resources {
    i16 power;
    i16 industry;
    i16 exchange;
};

bool HasEnoughResources (Resources stock, Resources cost) {

    if(stock.power      < cost.power) return false;
    if(stock.industry   < cost.industry) return false;
    if(stock.exchange   < cost.exchange) return false;

    return true;
}

Resources operator+ (Resources res1, Resources res2) {
    return {
        Clamp(res1.power      + res2.power, -MAX_RESOURCE, MAX_RESOURCE),
        Clamp(res1.industry   + res2.industry,-MAX_RESOURCE, MAX_RESOURCE),
        Clamp(res1.exchange   + res2.exchange,-MAX_RESOURCE, MAX_RESOURCE),
    };
}

Resources operator- (Resources res1, Resources res2) {
    return {
        res1.power      - res2.power,
        res1.industry   - res2.industry,
        res1.exchange   - res2.exchange,
    };
}

#endif