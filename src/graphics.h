struct Color {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

Color operator+ (Color color1, Color color2) {

    Color color = {
        color1.r + color2.r,
        color1.g + color2.g,
        color1.b + color2.b,
        color1.a + color2.a,
    };

    if(color.r < 0) color.r = 0;
    if(color.g < 0) color.g = 0;
    if(color.b < 0) color.b = 0;
    if(color.a < 0) color.a = 0;

    return color;
}

const Color ResourceBarColor = { 0, 200, 200, 255 };
const Color ResourceProductionColor = { 100, 200, 0, 255 };
const Color ToxicColor = { 20, 200, 0, 255 };

// standard colors
const Color White = { 20, 200, 0, 255 };
const Color Green = { 0, 255, 0, 255 };
const Color Red   = { 255, 0, 0, 255 };
const Color Cyan  = { 0, 0, 255, 255 };


