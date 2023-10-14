#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <vector>
#include <cmath>
#include <map>

namespace ccl
{
class CCL
{
public:
    CCL();
    ~CCL();

    bool connectedComponetsLabeling(const std::vector<Eigen::MatrixXd>& x, const std::vector<std::pair<Eigen::MatrixXd, double>>& score, Eigen::MatrixXi& labels);
    void firstScan();
    // int forward(const int& row, const int& col);

    bool inMatrix(const int& row, const int& col);
    bool isVaild(const int& row, const int& col);
    bool canConnect(const int& row, const int& col, const int& label);
    void connect(const int& row, const int& col, const int& label);
    void newRegion(const int& row, const int& col);
    int Tmin(const int& row, const int& col, const int mask[2][5]);
    void NoneOperation(const int& row, const int& col);

    void setConnectivity(int c) {
        if (c==4 || c==8)
        connectivity_ = c;
    }
private:
    enum {
        Right = 0,
        RRight = 1,
        Under=2,
        RUnder=3
    };

    enum {
        Left=0,
        LLeft=1,
        Above=2,
        LAbove=3
    };
    enum 
    {
        None = 0,
        Isolate = 1,
        Otherwise = 2,
    };
    const int f_mask[2][5] = {{0,-1,-1,-1, 0}, {0, 0, 1,-1,-1}};
    const int b_masl[2][5] = {{0, 1, 1, 1,-0}, {0, 0,-1, 1, 1}};

    int connectivity_ = 8;
    int threshold_ = 0.25;
    const std::vector<Eigen::MatrixXd>* x_;
    const std::vector<std::pair<Eigen::MatrixXd, double>>* score_; // score and threshold
    Eigen::MatrixXi* labels_;
    std::vector<int> tree_;
    int m_ = 1;
    int rows_, cols_;
};

};