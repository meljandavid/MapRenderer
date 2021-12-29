#include<list>
#include<cmath>

struct Point { int x,y; };

std::list<Point> generateMap(int N) {
    std::list<Point> buildings;
    int columns = int(std::log2(N-1)) + 1;
    int upperpow = (1 << (columns));

    for (int c = 0; c < columns; c++) {
        for (int y = (1<<c); y <= 2*upperpow; y++) {
            if ( ( (y - (1 << c) ) % (1 << (c + 2) ) ) < (1 << (c + 1) ) ) {
                buildings.push_back({ 2 * c,y });
                if (c != 0) buildings.push_back({ -2 * c,y });
            }
        }
    }

    int deletable = upperpow - N;
    for (int c = columns - 1; c >= 0; c--) {
        if (deletable >= (1 << c)) {
            deletable -= (1 << c);
            int y = (1 << c) + (1 << (c + 1));
            buildings.push_back({-2 * c, y});
        }
    }

    return buildings;
}

int main() {
    const std::list<Point>& map = generateMap(12);

    // valahogy megjelenitjuk a terkepet...

    return 0;
}
