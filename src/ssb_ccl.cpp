#include <ssb_ccl.h>
#include <iostream>
namespace ccl {
SSbCCL::SSbCCL(){}

SSbCCL::~SSbCCL(){}


// Check whether the cell has large enough values for all types of the scores. 
bool SSbCCL::isValid(const int& row, const int& col) const
{
    for (const auto& map: *score_) 
    {
        if (std::isnan(map.first(row, col))) return false; 
        if (map.first(row, col) < map.second) return false;
    }
    return true;
}

bool SSbCCL::inMatrix(const int& row, const int& col) const
{
    if (row >= rows_) return false;
    if (row < 0) return false;
    if (col >= cols_) return false;
    if (col < 0) return false;

    return true;
}

// check the connectivity according to the states. 
bool SSbCCL::canConnect(const int& r, const int& c, const int& label) const
{
    if (labels_->coeff(r,c)==0); // connect a cell
    else ; // connect a region
    return true;
}

// process connection between a region including cell(i,j) and another one labeled by label.
// update state of the region 
void SSbCCL::connect(const int& row, const int& col, const int& label)
{

}

// process initialize the state of the new region (new label).
void SSbCCL::newRegion(const int& row, const int& col)
{

}

int SSbCCL::Tmin(const int& row, const int& col, const int mask[2][5]) const
{
    int min_label=m_;
    for (int i=0; i<5; ++i) 
    {
        int r=row+mask[0][i];
        int c=col+mask[1][i];
        if (!inMatrix(r, c)) continue;
        const int& label = table_[labels_->coeffRef(r,c)];
        if (label == 0) continue;
        if (!canConnect(row, col, label)) continue;

        if (label < min_label) min_label = label;
    }
    return min_label;
}

void SSbCCL::firstScan()
{
    for (int i=0; i<rows_; ++i)
    {
        for (int j=0; j<cols_; ++j)
        {
            // std::cout << "Index: (" << i << "," << j << ")" << std::endl; 
            if (!isValid(i,j)) {    
                // std::cout << "The cell does not have vaild value. " << std::endl;
                labels_->coeffRef(i,j) = 0;
                continue;
            }

            std::vector<bool> is_labeled(4);
            for (int k=1; k<5; ++k) 
            {
                int r=i+f_mask_[0][k];
                int c=j+f_mask_[1][k];
                if (!inMatrix(r, c)) {
                    // std::cout << "cell (" << r << "," << c << ") is out of range" << std::endl;
                    is_labeled[k-1] = false;
                    continue;
                }
                if (labels_->coeff(r,c) != 0)
                {
                    if (canConnect(i,j, labels_->coeff(r,c))) is_labeled[k-1] = true;
                    else {
                        is_labeled[k-1] = false;
                        // std::cout << "cell (" << r << "," << c << ") cannot be connected" << std::endl;
                    }
                }
                else {
                    // std::cout << "cell (" << r << "," << c << ") is not labeled" << std::endl;
                    is_labeled[k-1] = false;
                }
                
            }
            // all of cells in the mask do not have a label. 
            if (std::all_of(is_labeled.begin(), is_labeled.end(), [](bool x){return !x;})) 
            {
                // std::cout << "all of cells in the mask do not have a label." << std::endl;
                labels_->coeffRef(i,j) = m_;
                table_.push_back(m_);
                ++m_;
                newRegion(i,j);
            }
            // at least one of the cells in the mask have a label.  
            else 
            {
                // std::cout << "at least one of the cells in the mask have a label." << std::endl;
                int t_min = Tmin(i, j, f_mask_);
                if (t_min >= m_) 
                {
                    // std::cout << "Tmin: " << t_min << std::endl;
                    continue;
                }
                
                labels_->coeffRef(i, j) = t_min;
                connect(i,j, labels_->coeffRef(i,j));
                for (int k=1; k<5; ++k)
                {
                    int r=i+f_mask_[0][k];
                    int c=j+f_mask_[1][k];
                    if (is_labeled[k-1]) 
                    {
                        table_[labels_->coeff(r, c)] = t_min; // Tmin
                        connect(r, c, t_min);
                    }
                }
            }
            // std::cout << "is_labeled: ";
            // for (int k=0; k<4; ++k) std::cout << is_labeled[k] << " ";
            // std::cout << std::endl;
            // std::cout << "lables: " << std::endl;
            // std::cout << *labels_ << std::endl;
            // std::cout << "tree: ";
            // for (const auto& t: table_) std::cout << t << " ";
            // std::cout << std::endl;
            // std::cout << "m: " << m_ << std::endl;

            // std::cout << std::endl;
        }
    }
}

bool SSbCCL::scan(const int& row, const int& col, const int mask[2][5])
{
    if (labels_->coeffRef(row,col) == 0) return false;
    int new_label = Tmin(row, col, mask);
    if (new_label == m_) return false;

    bool change_label = false;
    // update label matrix
    if (new_label != labels_->coeffRef(row, col))
    {
        change_label = true;
        labels_->coeffRef(row,col) = new_label;
        connect(row,col, labels_->coeffRef(row,col));
    }

    // update label table
    for (int k=1; k<5; ++k)
    {
        int r=row+mask[0][k];
        int c=col+mask[1][k];
        if (!inMatrix(r, c)) continue;
        if (labels_->coeff(r,c)==0) continue;
        
        if (table_[labels_->coeff(r, c)] != labels_->coeff(row,col))
        {
            if (canConnect(row, col, labels_->coeff(r, c)))
            {
                change_label = true;
                table_[labels_->coeff(r, c)] = labels_->coeffRef(row,col); // Tmin
                connect(r, c, labels_->coeff(row,col));
            }
        }
    }

    // std::cout << "m: " << m_ << std::endl;
    // std::cout << "new_label: " << new_label << std::endl;
    // std::cout << "table: ";
    // for (int i=0; i<table_.size(); ++i) std::cout << table_[i] << " ";
    // std::cout << std::endl;
    // std::cout << "(i,j): (" << row << "," << col << ")" << std::endl; 
    // std::cout << *labels_ << std::endl;
    

    return change_label;
}

// process forward scan. 
// Return whether the labels changed
bool SSbCCL::forwardScan()
{
    bool change_labels = false;
    for (int i=0;i<rows_; ++i)
    {
        for (int j=0; j<cols_; ++j)
        {
            change_labels |= scan(i,j, f_mask_);
        }
    }

    return change_labels;
}

bool SSbCCL::backwardScan()
{
    bool change_labels = false;
    for (int i=rows_-1; i>=0; --i)
    {
        for (int j=cols_-1; j>=0; --j) 
        {
            change_labels |= scan(i,j, b_mask_);
        }
    }

    return change_labels;
}

bool SSbCCL::initialize(const StateMatrix& state, const ScoreMatrix& score, LabelMatrix& labels)
{
    if (score.size() == 0) return false;
    rows_ = score[0].first.rows();
    cols_ = score[0].first.cols();

    if (state.size() != rows_ || !std::all_of(state.begin(), state.end(), [this](const auto& x){return x.size() == cols_;})) 
    {
        std::cout << "matrixes in x have different size." << std::endl;
        return false;
    }

    // set vars
    state_ = &state;
    score_ = &score;
    labels_ = &labels;
    labels_->resize(rows_, cols_);
    labels_->setZero();
    table_.resize(1);
    table_[0] = std::numeric_limits<int>::infinity();
    m_ = 1;

    return true;
}

}