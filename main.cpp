#include <iostream>
#include "numeric"
#include "screen.h"

struct vec3
{
    float x, y, z;
};

void rotate(vec3 &point, float x = 1, float y = 1, float z = 1)
{
    float rad = 0;

    rad = x;
    point.y = std::cos(rad) * point.y - std::sin(rad) * point.z;
    point.z = std::sin(rad) * point.y + std::cos(rad) * point.z;

    rad = y;
    point.y = std::cos(rad) * point.x + std::sin(rad) * point.z;
    point.z = std::sin(rad) * point.x + std::cos(rad) * point.z;

    rad = z;
    point.x = std::cos(rad) * point.x - std::sin(rad) * point.y;
    point.y = std::sin(rad) * point.x + std::cos(rad) * point.y;
}

void line(Screen &screen, float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    float length = std::sqrt(dx * dx + dy * dy);
    float angle = std::atan2(dy, dx);

    for (float i = 0; i < length; i++)
    {
        screen.pixel(
            x1 + std::cos(angle) * i,
            y1 + std::sin(angle) * i);
    }
}

int main(void)
{
    Screen screen;

    std::vector<vec3> points{
        {100, 100, 100},
        {200, 100, 100},
        {200, 200, 100},
        {100, 200, 100},

        {100, 100, 200},
        {200, 100, 200},
        {200, 200, 200},
        {100, 200, 200}};

    // calac centroid
    vec3 c{0, 0, 0};
    for (auto &p : points)
    {
        c.x += p.x;
        c.y += p.y;
        c.z += p.z;
    }

    c.x /= points.size();
    c.y /= points.size();
    c.z /= points.size();

    while (true)
    {
        for (auto &p : points)
        {
            p.x -= c.x;
            p.y -= c.y;
            p.z -= c.z;
            rotate(p, 0.002, 0.001, 0.004);
            p.x += c.x;
            p.y += c.y;
            p.z += c.z;

            screen.pixel(p.x, p.y);
        }

        screen.show();
        screen.clear();
        screen.input();
        SDL_Delay(30);
    }

    screen.~Screen();

    return 0;
}