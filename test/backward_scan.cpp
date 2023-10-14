#include <ccl.h>
#include <iostream>
int main()
{
    ccl::CCL ccl_solver;
    Eigen::MatrixXd s(7, 7);
    s << 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 0, 1,
        1, 0, 0, 0, 1, 0, 1, 
        1, 0, 1, 1, 1, 0, 1, 
        1, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1;

    Eigen::MatrixXi labels;
    std::vector<std::vector<Eigen::VectorXd>> xx(7);
    for (int i=0; i<xx.size(); ++i) {
        xx[i].resize(7);
        for (int j=0; j<xx[i].size(); ++j)
            xx[i][j].resize(3);
    }
    std::vector<std::pair<Eigen::MatrixXd, double>> scores;
    scores.emplace_back(std::make_pair(s, 0.5));
    ccl_solver.initialize(xx, scores, labels);
    ccl_solver.firstScan();
    ccl_solver.backwardScan();

    std::cout << labels << std::endl;
    return 0;
}