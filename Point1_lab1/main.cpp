#include <set>
#include "gmsh.h"
#include <cmath>
#include <iostream>

int main(int argc, char **argv) {
    double pi = 3.141592653589793;
    //std::cout<<"HELLO" << 1%2;
    gmsh::initialize();
    gmsh::model::add("t1");
    double lc = 1e-1;
    double R = 2;
    double r_out = .8;
    double r_in = .3;

    // внешние окружности в плоскости ХУ
    gmsh::model::geo::addPoint(0, 0, 0, lc, 1);
    gmsh::model::geo::addPoint(R-r_out, 0, 0, lc, 2);
    gmsh::model::geo::addPoint(-(R-r_out), 0, 0, lc, 3);
    gmsh::model::geo::addCircleArc(2, 1, 3, 1);
    gmsh::model::geo::addCircleArc(3, 1, 2, 2);
    gmsh::model::geo::addPoint(R+r_out, 0, 0, lc, 4);
    gmsh::model::geo::addPoint(-(R+r_out), 0, 0, lc, 5);
    gmsh::model::geo::addCircleArc(4, 1, 5, 3);
    gmsh::model::geo::addCircleArc(5, 1, 4, 4);

    // внутренние окружности в плоскости ХУ
    gmsh::model::geo::addPoint(R, 0, 0, lc, 6);
    gmsh::model::geo::addPoint(R-r_in, 0, 0, lc, 7);
    gmsh::model::geo::addPoint(-(R-r_in), 0, 0, lc, 8);
    gmsh::model::geo::addCircleArc(7, 1, 8, 5);
    gmsh::model::geo::addCircleArc(8, 1, 7, 6);
    gmsh::model::geo::addPoint(R+r_in, 0, 0, lc, 9);
    gmsh::model::geo::addPoint(-(R+r_in), 0, 0, lc, 10);
    gmsh::model::geo::addCircleArc(9, 1, 10, 7);
    gmsh::model::geo::addCircleArc(10, 1, 9, 8);

    // внешние окружности в плоскости XZ
    gmsh::model::geo::addPoint(R, 0, r_out, lc, 11);
    gmsh::model::geo::addPoint(R, 0, -r_out, lc, 12);
    gmsh::model::geo::addCircleArc(4, 6, 11, 9);
    gmsh::model::geo::addCircleArc(11, 6, 2, 10);
    gmsh::model::geo::addCircleArc(2, 6, 12, 11);
    gmsh::model::geo::addCircleArc(12, 6, 4, 12);

    gmsh::model::geo::addPoint(-R, 0, 0, lc, 13);
    gmsh::model::geo::addPoint(-R, 0, r_out, lc, 14);
    gmsh::model::geo::addPoint(-R, 0, -r_out, lc, 15);
    gmsh::model::geo::addCircleArc(5, 13, 14, 13);
    gmsh::model::geo::addCircleArc(14, 13, 3, 14);
    gmsh::model::geo::addCircleArc(3, 13, 15, 15);
    gmsh::model::geo::addCircleArc(15, 13, 5, 16);


    // внутренние окружности в плоскости XZ
    gmsh::model::geo::addPoint(R, 0, r_in, lc, 16);
    gmsh::model::geo::addPoint(R, 0, -r_in, lc, 17);

    gmsh::model::geo::addCircleArc(9, 6, 16, 17);
    gmsh::model::geo::addCircleArc(16, 6, 7, 18);
    gmsh::model::geo::addCircleArc(7, 6, 17, 19);
    gmsh::model::geo::addCircleArc(17, 6, 9, 20);

    gmsh::model::geo::addPoint(-R, 0, r_in, lc, 18);
    gmsh::model::geo::addPoint(-R, 0, -r_in, lc, 19);
    gmsh::model::geo::addCircleArc(10, 13, 18, 21);
    gmsh::model::geo::addCircleArc(18, 13, 8, 22);
    gmsh::model::geo::addCircleArc(8, 13, 19, 23);
    gmsh::model::geo::addCircleArc(19, 13, 10, 24);


    // Окружности в плоскости z = +- r_out, z = +- r_in
    gmsh::model::geo::addPoint(0, 0, r_out, lc, 20);
    gmsh::model::geo::addPoint(0, 0, -r_out, lc, 21);
    gmsh::model::geo::addPoint(0, 0, r_in, lc, 22);
    gmsh::model::geo::addPoint(0, 0, -r_in, lc, 23);
    gmsh::model::geo::addCircleArc(16, 22, 18, 29);
    gmsh::model::geo::addCircleArc(18, 22, 16, 30);
    gmsh::model::geo::addCircleArc(17, 23, 19, 31);
    gmsh::model::geo::addCircleArc(19, 23, 17, 32);
    gmsh::model::geo::addCircleArc(11, 20, 14, 25);
    gmsh::model::geo::addCircleArc(14, 20, 11, 26);
    gmsh::model::geo::addCircleArc(12, 21, 15, 27);
    gmsh::model::geo::addCircleArc(15, 21, 12, 28);

    //поверхность внешнего
    gmsh::model::geo::addCurveLoop({-13,4,9,-26}, 1);
    gmsh::model::geo::addSurfaceFilling({1}, 1);
    gmsh::model::geo::addCurveLoop({-9, 3, 13, -25}, 2);
    gmsh::model::geo::addSurfaceFilling({2}, 2);
    gmsh::model::geo::addCurveLoop({16, -3, -12, 27}, 3);
    gmsh::model::geo::addSurfaceFilling({3}, 3);
    gmsh::model::geo::addCurveLoop({12, -4, -16, 28}, 4);
    gmsh::model::geo::addSurfaceFilling({4}, 4);
    gmsh::model::geo::addCurveLoop({-10,-26,14,2}, 5);
    gmsh::model::geo::addSurfaceFilling({5}, 5);
    gmsh::model::geo::addCurveLoop({-14, -25, 10, 1}, 6);
    gmsh::model::geo::addSurfaceFilling({6}, 6);
    gmsh::model::geo::addCurveLoop({15, 28, -11, -2}, 7);
    gmsh::model::geo::addSurfaceFilling({7}, 7);
    gmsh::model::geo::addCurveLoop({11, 27, -15, -1}, 8);
    gmsh::model::geo::addSurfaceFilling({8}, 8);

    //поверхность внутреннего
    gmsh::model::geo::addCurveLoop({19, 31, -23, -5}, 9);
    gmsh::model::geo::addSurfaceFilling({9}, 9);
    gmsh::model::geo::addCurveLoop({23, 32, -19, -6}, 10);
    gmsh::model::geo::addSurfaceFilling({10}, 10);
    gmsh::model::geo::addCurveLoop({-22, -29, 18, 5}, 11);
    gmsh::model::geo::addSurfaceFilling({11}, 11);
    gmsh::model::geo::addCurveLoop({-18, -30, 22, 6}, 12);
    gmsh::model::geo::addSurfaceFilling({12}, 12);
    gmsh::model::geo::addCurveLoop({-21, 8, 17, -30}, 13);
    gmsh::model::geo::addSurfaceFilling({13}, 13);
    gmsh::model::geo::addCurveLoop({20, -8, -24, 32}, 14);
    gmsh::model::geo::addSurfaceFilling({14}, 14);
    gmsh::model::geo::addCurveLoop({21, -29, -17, 7}, 15);
    gmsh::model::geo::addSurfaceFilling({15}, 15);
    gmsh::model::geo::addCurveLoop({-20, 31, 24, -7}, 16);
    gmsh::model::geo::addSurfaceFilling({16}, 16);

    // Объединение
    gmsh::model::geo::addSurfaceLoop({1, 2, 3, 4, 5, 6, 7, 8}, 1);
    gmsh::model::geo::addSurfaceLoop({9, 10, 11, 12, 13, 14, 15, 16}, 2);
    gmsh::model::geo::addVolume({1, 2}, 1);
    gmsh::model::geo::synchronize();
    gmsh::model::mesh::generate(3);
    gmsh::write("torus.msh");
    gmsh::write("torus.stl");
    std::set<std::string> args(argv, argv + argc);
    if(!args.count("-nopopup")) gmsh::fltk::run();
    gmsh::finalize();
    return 0;
}
