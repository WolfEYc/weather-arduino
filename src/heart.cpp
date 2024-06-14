#include "heart.h"

namespace heart
{
    ArduinoLEDMatrix matrix;

    byte frame[R_MAX][C_MAX] = {
        {0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0},
        {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    struct Point
    {
        int r;
        int c;

        static Point init(int r, int c)
        {
            Point pt;
            pt.r = r;
            pt.c = c;
            return pt;
        }

        Point operator+(const Point other) const
        {
            Point newpt;
            newpt.r = r + other.r;
            newpt.c = c + other.c;
            return newpt;
        }

        bool operator==(const Point other) const
        {
            return r == other.r && c == other.c;
        }
        bool operator!=(const Point other) const
        {
            return !(*this == other);
        }

        bool in_range()
        {
            return !(r < 0 || c < 0 || r >= R_MAX || c >= C_MAX);
        }

        bool is_on()
        {
            return in_range() && frame[r][c] == 1;
        }
    };

    Point find_next_pt(Point curr_pt, Point prev_pt)
    {
        for (int r = -1; r <= 1; r++)
        {
            for (int c = -1; c <= 1; c++)
            {
                Point pt = curr_pt + Point::init(r, c);
                if (!pt.is_on() || pt == prev_pt || pt == curr_pt)
                {
                    continue;
                }
                // this is the point we are lookin for
                return pt;
            }
        }
        return Point::init(-1, -1);
    }

    Point curr_pt = Point::init(2, 5);
    Point prev_pt = Point::init(1, 4);

    void setup()
    {
        matrix.begin();
    }

    void loop()
    {
        Point next_pt = find_next_pt(curr_pt, prev_pt);
        prev_pt = curr_pt;
        curr_pt = next_pt;
        frame[prev_pt.r][prev_pt.c] = 1;
        frame[curr_pt.r][curr_pt.c] = 0;
        matrix.renderBitmap(frame, R_MAX, C_MAX);
    }
}
