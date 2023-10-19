#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <vector>
#include <cmath>
#include <map>

namespace ccl
{

using Matrix = Eigen::MatrixXf;
using Vector = Eigen::VectorXf;
using ScoreMatrix = std::vector<std::pair<Matrix, float>>;
using StateMatrix = std::vector<std::vector<Vector>>;
class SSbCCL
{
public:
    SSbCCL();
    ~SSbCCL();

    bool initialize(const StateMatrix& state, const ScoreMatrix& score, Matrix& labels);
    void firstScan();
    bool forwardScan();
    bool backwardScan();
    bool scan(const int& row, const int& col, const int mask[2][5]);

    bool inMatrix(const int& row, const int& col) const;
    bool isVaild(const int& row, const int& col) const;
    int Tmin(const int& row, const int& col, const int mask[2][5]) const;

    // check the connectivity according to the states. 
    virtual bool canConnect(const int& row, const int& col, const int& label) const;

    // process connection between a region including cell(i,j) and another one labeled by label.
    // update state of the region 
    virtual void connect(const int& row, const int& col, const int& label);
    
    // process initialize the state of the new region (new label).
    virtual void newRegion(const int& row, const int& col);
    

protected:
    const int f_mask_[2][5] = {{0,-1,-1,-1, 0}, {0, 0, 1,-1,-1}};
    const int b_mask_[2][5] = {{0, 1, 1, 1, 0}, {0, 0,-1, 1, 1}};

    const StateMatrix* state_;
    const ScoreMatrix* score_; // score and threshold
    Matrix* labels_;
    std::vector<int> table_;
    int m_ = 1;
    int rows_, cols_;
};

};