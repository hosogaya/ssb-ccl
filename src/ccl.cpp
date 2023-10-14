#include <ccl.h>
#include <iostream>
namespace ccl {
CCL::CCL(){}

CCL::~CCL(){}


// Check whether the cell has large enough values for all types of the scores. 
bool CCL::isVaild(const int& row, const int& col) 
{
    for (const auto& map: *score_) 
    {
        if (map.first(row, col) < map.second) return false;
    }
    return true;
}

bool CCL::inMatrix(const int& row, const int& col)
{
    if (row >= rows_) return false;
    if (row < 0) return false;
    if (col >= cols_) return false;
    if (col < 0) return false;

    return true;
}

// check the connectivity according to some indexes. 
bool CCL::canConnect(const int& r, const int& c, const int& label)
{
    if (labels_->coeff(r,c)==0); // connect a cell
    else ; // connect a region
    return true;
}

// process connection between a region including cell(i,j) and another one labeled by label. 
void CCL::connect(const int& row, const int& col, const int& label)
{

}

void CCL::newRegion(const int& row, const int& col)
{

}

int CCL::Tmin(const int& row, const int& col, const int mask[2][5])
{
    int min_label=m_;
    for (int i=0; i<5; ++i) 
    {
        int r=row+mask[0][i];
        int c=col+mask[1][i];
        if (!inMatrix(r, c)) continue;
        int& label = labels_->coeffRef(r,c);
        if (label == 0) continue;
        if (!canConnect(r, c, label)) continue;

        if (tree_[label] < min_label) min_label = label;
    }
    return min_label;

}

void CCL::NoneOperation(const int& row, const int& col)
{
    labels_->coeffRef(row, col) = 0;
}

// int CCL::forward(const int& row, const int& col)
// {
//     if (isNAN(row, col)) {
//         return None;
//     }
//     std::vector<bool> is_NAN(4);
//     for (int i=1; i<5; ++i) 
//     {
//         is_NAN[i-1] = isNAN(row+f_mask[0][i], col+f_mask[0][i]);
//     }
//     // all of cells in the mask has NAN value
//     if (std::all_of(is_NAN.begin(), is_NAN.end(), [](bool x){return x;})) return Isolate;

//     return Otherwise;
// }

void CCL::firstScan()
{
    for (int i=0; i<rows_; ++i)
    {
        for (int j=0; j<cols_; ++j)
        {
            if (!isVaild(i,j)) {    
                std::cout << "The cell does not have vaild value. " << std::endl;
                labels_->coeffRef(i,j) = 0;
                continue;
            }

            std::vector<bool> is_labeled(4);
            for (int k=1; k<5; ++k) 
            {
                int r=i+f_mask[0][k];
                int c=j+f_mask[1][k];
                if (!inMatrix(r, c)) {
                    is_labeled[k-1] = false;
                    continue;
                }
                is_labeled[k-1] = labels_->coeffRef(r, c) != 0;
            }
            for (int k=0; k<4; ++k) std::cout << is_labeled[k] << " ";
            std::cout << std::endl;
            // all of cells in the mask do not have a label. 
            if (std::all_of(is_labeled.begin(), is_labeled.end(), [](bool x){return !x;})) 
            {
                std::cout << "all of cells in the mask do not have a label." << std::endl;
                labels_->coeffRef(i,j) = m_;
                tree_.push_back(m_);
                ++m_;
            }
            // at least one of the cells in the mask have a label.  
            else 
            {
                std::cout << "at least one of the cells in the mask have a label." << std::endl;
                labels_->coeffRef(i, j) = Tmin(i, j, f_mask);
                connect(i,j, labels_->coeffRef(i,j));
                for (int k=1; k<5; ++k)
                {
                    int r=i+f_mask[0][k];
                    int c=j+f_mask[1][k];
                    if (is_labeled[k-1]) 
                    {
                        if (canConnect(r, c, labels_->coeff(r, c)))
                        tree_[labels_->coeff(r, c)] = labels_->coeffRef(i,j); // Tmin
                    }
                }
            }
            std::cout << "lables: " << std::endl;
            std::cout << *labels_ << std::endl;
            std::cout << "tree: ";
            for (const auto& t: tree_) std::cout << t << " ";
            std::cout << std::endl;
            std::cout << "m: " << m_ << std::endl;

            std::cout << std::endl;
        }
    }
}

bool CCL::connectedComponetsLabeling(const std::vector<Eigen::MatrixXd>& x, const std::vector<std::pair<Eigen::MatrixXd, double>>& score, Eigen::MatrixXi& labels)
{
    rows_ = x[0].rows();
    cols_ = x[0].cols();

    if (!std::all_of(x.begin(), x.end(), [this](const Eigen::MatrixXd& mat){return (mat.rows()==rows_) && (mat.cols()==cols_);})) 
    {
        std::cout << "matrixes in x have different size." << std::endl;
        return false;
    }

    // set vars
    x_ = &x;
    score_ = &score;
    labels_ = &labels;
    labels_->setZero();
    labels_->resize(rows_, cols_);
    tree_.resize(1);
    tree_[0] = std::numeric_limits<int>::infinity();
    m_ = 1;

    firstScan();
}

}