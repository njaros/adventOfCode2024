#include "utils/helpers.hpp"

bool almostEqual(long long a, double b) {
    double epsilon = 0.001;

    if (a + epsilon > b && a - epsilon < b)
        return true;
    return false;
}

ll searchLowerPrice(const Coord *machine)
{
    double epsilon = 0.001;
    Vector XY({
        machine[2].first,
        machine[2].second
    });
    Matrix buttons({
        {machine[0].first, machine[1].first},
        {machine[0].second, machine[1].second}
    });

    Vector ab = buttons.inverse() * XY;
    
    ll a = ab[0] + epsilon;
    ll b = ab[1] + epsilon;

    if (almostEqual(a, ab[0]) && almostEqual(b, ab[1])) {
        return 3 * a + b;
    }

    return 0;
}

int main() {
    ui part = 0;
    std::ifstream input;
    inputLib::extracted extraced;
    Coord machine[] = {Coord(0, 0), Coord(0, 0), Coord(0, 0)};
    ll res = 0;

    if (getFileAndPart(13, input, part))
        return errno;

    while (extraced.second != EOF)
    {
        for (int i = 0; i < 3; ++i) {
            if (i == 2 && part != 1) {
                extraced = inputLib::extractNextNumber(input);
                machine[i].first = extraced.first.value() + 10000000000000;
                extraced = inputLib::extractNextNumber(input);
                machine[i].second = extraced.first.value() + 10000000000000;
            }
            else {
                extraced = inputLib::extractNextNumber(input);
                machine[i].first = extraced.first.value();
                extraced = inputLib::extractNextNumber(input);
                machine[i].second = extraced.first.value();
            }
        }
        res += searchLowerPrice(machine);
    }

    std::cout << "result is " << res << '\n';
}