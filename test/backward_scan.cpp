#include <ccl.h>
#include <iostream>
int main()
{
    ccl::CCL ccl_solver;
    Eigen::MatrixXd x(7,7), y(7,7), z(7,7), s(7, 7);
    x << 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1;
    y = x;
    z = x;
    s << 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 0, 1,
        1, 0, 0, 0, 1, 0, 1, 
        1, 0, 1, 1, 1, 0, 1, 
        1, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1;

    Eigen::MatrixXi labels;
    std::vector<Eigen::MatrixXd> xx{x, y, z};
    std::vector<std::pair<Eigen::MatrixXd, double>> scores;
    scores.emplace_back(std::make_pair(s, 0.5));
    ccl_solver.initialize(xx, scores, labels);
    ccl_solver.firstScan();
    ccl_solver.backwardScan();

    return 0;
}