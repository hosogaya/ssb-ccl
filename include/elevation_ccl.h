#include <ssb_ccl.h>

namespace ccl
{
class ElevationCCL : public SSbCCL
{
public:
    // check the connectivity according to the states. 
    bool canConnect(const int& row, const int& col, const int& label) const override;

    // process connection between a region including cell(i,j) and another one labeled by label.
    // update state of the region 
    void connect(const int& row, const int& col, const int& label) override;
    
    // process initialize the state of the new region (new label).
    void newRegion(const int& row, const int& col) override;
private:
    struct Region 
    {
        int label_;
        int component_num_;
        Eigen::VectorXd mean_;
        // Eigen::MatrixXd variance_;
        Region(int label, const Eigen::VectorXd& s) 
        {
            label_ = label;
            component_num_ = 1;
            mean_ = s;
            // variance_.resize(s.size(), s.size());
            // variance_.setZero();
        }
        void addCell(const Eigen::VectorXd& s)
        {
            mean_ = (mean_*component_num_ + s)/(++component_num_);
        }
        void addRegion(const Region& r)
        {
            mean_ = (mean_*component_num_ + r.mean_*r.component_num_)/(component_num_ + r.component_num_);
            component_num_ += r.component_num_;
        }
    };
    std::vector<Region> regions_;
    double threshold_distance_;
};
}