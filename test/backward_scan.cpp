#include <ssb_ccl.h>
#include <iostream>
int main()
{
    ccl::SSbCCL ccl_solver;
    ccl::Matrix s(7, 7);
    s << 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 0, 1,
        1, 0, 0, 0, 1, 0, 1, 
        1, 0, 1, 1, 1, 0, 1, 
        1, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1;

    ccl::LabelMatrix labels;
    ccl::StateMatrix xx(7);
    for (int i=0; i<xx.size(); ++i) {
        xx[i].resize(7);
        for (int j=0; j<xx[i].size(); ++j)
            xx[i][j].resize(3);
    }
    ccl::ScoreMatrix scores;
    scores.emplace_back(std::make_pair(s, 0.5));
    ccl_solver.initialize(xx, scores, labels);
    ccl_solver.firstScan();
    ccl_solver.backwardScan();

    std::cout << labels << std::endl;
    return 0;
}