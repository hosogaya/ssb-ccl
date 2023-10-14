#include <elevation_ccl.h>

namespace ccl
{
bool ElevationCCL::canConnect(const int& row, const int& col, const int& label) const
{
    const int cur_label = labels_->coeff(row, col);
    // connect with a cell
    if (cur_label==0)
    {   
        const Eigen::VectorXd& state = state_->at(row).at(col);
        if ((regions_[label].mean_ - state).squaredNorm() < threshold_distance_) return true;
        else return false;
    }
    else // connect with a region 
    {
        if ((regions_[label].mean_ - regions_[cur_label].mean_).squaredNorm() < threshold_distance_) return true;
        else return false;
    }
    
    return false;
}

void ElevationCCL::connect(const int& row, const int& col, const int& label)
{
    const int cur_label = labels_->coeff(row, col);
    // connect with a cell
    if (cur_label==0)
    {   
        const Eigen::VectorXd& state = state_->at(row).at(col);
        regions_[label].addCell(state);
    }
    else // connect with a region 
    {
        if (label < cur_label) regions_[label].addRegion(regions_[cur_label]);
        else  regions_[cur_label].addRegion(regions_[label]);
    }    
}
    
void ElevationCCL::newRegion(const int& row, const int& col) 
{
    int size = state_->at(row).at(col).size();
    regions_.emplace_back(regions_.size(), state_->at(row).at(col));
}
}