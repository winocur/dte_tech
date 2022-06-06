
float Clamp (float value, float min, float max) {
    return (value < min) ? min : (value > max) ? max : value;
}

float Abs (float value) {
    return (value < 0) ? -value : value;
}