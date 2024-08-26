#include <iostream>
#include "numeric"
#include "screen.h"

char boat = 'o';

struct vec3
{
    float x, y, z;
};

struct connection
{
    int a, b;
};

void rotate(vec3 &point, float x = 1, float y = 1, float z = 1)
{
    float radX = x;
    float radY = y;
    float radZ = z;

    // Rotate around X-axis
    float cosX = std::cos(radX);
    float sinX = std::sin(radX);
    point.y = cosX * point.y - sinX * point.z;
    point.z = sinX * point.y + cosX * point.z;

    // Rotate around Y-axis
    float cosY = std::cos(radY);
    float sinY = std::sin(radY);
    point.x = cosY * point.x + sinY * point.z;
    point.z = -sinY * point.x + cosY * point.z;

    // Rotate around Z-axis
    float cosZ = std::cos(radZ);
    float sinZ = std::sin(radZ);
    point.x = cosZ * point.x - sinZ * point.y;
    point.y = sinZ * point.x + cosZ * point.y;
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

    std::vector<connection> connections{
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7},

        {0, 1},
        {1, 2},
        {2, 3},
        {3, 0},

        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4},

    };

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

        for (auto &conn : connections)
        {
            line(screen,
                 points[conn.a].x,
                 points[conn.a].y,
                 points[conn.b].x,
                 points[conn.b].y);
        }

        screen.show();
        screen.clear();
        screen.input();
        SDL_Delay(30);
    }

    screen.~Screen();

    return 0;
}