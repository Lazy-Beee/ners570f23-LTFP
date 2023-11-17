#include "compare.hpp"
#include "src/Common.hpp"
#include <string>

using namespace std;
using namespace LTFP;

void poly(Real x, vector<Real> coeff)
{
    static int count = 0;
    count++;
    Real val = pow(x, 3) * coeff[0] + pow(x, 2) * coeff[1] + pow(x, 1) * coeff[2] + pow(x, 0) * coeff[3];
    COMPARE<Real>(computePoly(x, coeff), val, 1e-3, "polynomial " + to_string(count));
}

int main()
{
    poly(11.4, {1, 9, 1, 9});
    poly(5.14, {0, 8, 1, 0});

    PiecewisePoly poly;
    poly.first = {0, 100, 200};
    poly.second.push_back({2.0f, 0.0f});
    poly.second.push_back({1.0f, 100.0f});

    COMPARE(computePiecewisePoly(-1.0f, poly), 0.0f, 1e-3f, "piecewise polynomial 1");
    COMPARE(computePiecewisePoly(15.0f, poly), computePoly(15.0f, poly.second[0]), 1e-3f, "piecewise polynomial 2");
    COMPARE(computePiecewisePoly(115.0f, poly), computePoly(115.0f, poly.second[1]), 1e-3f, "piecewise polynomial 3");
    COMPARE(computePiecewisePoly(215.0f, poly), 300.0f, 1e-3f, "piecewise polynomial 4");

    Table table = tabulatePiecewisePoly(poly, {9.9f, 20.0f});

    // cout << "I\tX\tY" << endl;
    // for (size_t i; i < table.first.size(); i++)
    //     cout << i << " \t" << table.first[i] << " \t" << table.second[i] << endl;

    COMPARE(lookupTable(-1.0f, table), 0.0f, 1e-3f, "table lookup 1");
    COMPARE(lookupTable(15.0f, table), computePoly(15.0f, poly.second[0]), 1e-3f, "table lookup 2");
    COMPARE(lookupTable(115.0f, table), computePoly(115.0f, poly.second[1]), 1e-3f, "table lookup 3");
    COMPARE(lookupTable(215.0f, table), 300.0f, 1e-3f, "table lookup 4");

    COMPARE(lookupTable(-1.0f, table, true), 0.0f, 1e-3f, "inverse table lookup 1");
    COMPARE(lookupTable(30.0f, table, true), 15.0f, 1e-3f, "inverse table lookup 2");
    COMPARE(lookupTable(215.0f, table, true), 115.0f, 1e-3f, "inverse table lookup 3");
    COMPARE(lookupTable(315.0f, table, true), 200.0f, 1e-3f, "inverse table lookup 4");

    COMPARE_summary();
}