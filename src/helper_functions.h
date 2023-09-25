#ifndef __HELPER_FUNCTIONS__
#define __HELPER_FUNCTIONS__

#include <iostream>
#include <vector>
#include <string>

#include <Rcpp.h>

//' Retrieves the indexes of the rows that hold a specific value
//' in a specified column
inline std::vector<int> getRowIndexesForQuery(
    std::vector<std::vector<std::string>> mat,
    int col_of_interest,
    std::string query)
{
    std::vector<int> rowIndexes;
    for (int i = 0; i < mat.size(); ++i)
    {
        std::vector<std::string> row = mat[i];
        if (row[col_of_interest] == query)
        {
            rowIndexes.push_back(i);
        }
    }
    return rowIndexes;
}

//' Takes an integer vector and translates this to a Rcpp::IntegerVector
//' object so that it can be returned to R
inline Rcpp::IntegerVector getIntVecR(
    std::vector<int> cppVec)
{
    Rcpp::IntegerVector rVec;
    for (int i = 0; i < cppVec.size(); ++i)
    {
        rVec.push_back(cppVec[i]);
    }
    return rVec;
}

//' Takes a vector of vectors object (type string) and converts this
//' into the R equivalent (Rcpp::StringMatrix)
inline Rcpp::StringMatrix getStrMatrixR(
    std::vector<std::vector<std::string>> cppMat)
{
    int numRows = cppMat.size();
    int numCols = cppMat[0].size();

    if (numRows == 0 && numCols == 0)
    {
        return R_NilValue;
    }

    Rcpp::StringMatrix rMat(numRows, numCols);

    for (int i = 0; i < numRows; ++i)
    { // loop rows
        for (int j = 0; j < numCols; ++j)
        {
            rMat(i, j) = cppMat[i][j]; // set cell of matrix to correct value
        }
    }
    return rMat;
}

//' Takes a vector of vectors object (type double) and converts this
//' into the R equivalent (Rcpp::NumericMatrix)
inline Rcpp::NumericMatrix getNumericMatrixR(
    std::vector<std::vector<double>> cppMat)
{
    int numRows = cppMat.size();
    int numCols = cppMat[0].size();

    Rcpp::NumericMatrix rMat(numRows, numCols);

    for (int i = 0; i < numRows; ++i)
    { // loop rows
        for (int j = 0; j < numCols; ++j)
        {
            rMat(i, j) = cppMat[i][j]; // set cell of matrix to correct value
        }
    }
    return rMat;
}

//' Takes a Rcpp::StringMatrix object and and converts this to the C++
//' equivalent (a vector of vectors of type string)
inline std::vector<std::vector<std::string>> getStrMatrixCpp(
    Rcpp::StringMatrix rMat)
{
    int nrow = rMat.nrow();
    int ncol = rMat.ncol();

    std::vector<std::vector<std::string>> cppMat(nrow, std::vector<std::string>(ncol, ""));

    for (int i = 0; i < nrow; ++i)
    {
        for (int j = 0; j < ncol; ++j)
        {
            cppMat[i][j] = rMat(i, j);
        }
    }

    return cppMat;
}

//' Takes a Rcpp::NumericMatrix object and and converts this to the C++
//' equivalent (a vector of vectors of type double)
inline std::vector<std::vector<double>> getDoubleMatrixCpp(
    Rcpp::NumericMatrix rMat)
{
    int nrow = rMat.nrow();
    int ncol = rMat.ncol();

    std::vector<std::vector<double>> cppMat(nrow, std::vector<double>(ncol, 0.0));

    for (int i = 0; i < nrow; ++i)
    {
        for (int j = 0; j < ncol; ++j)
        {
            cppMat[i][j] = rMat(i, j);
        }
    }
    return cppMat;
}

#endif // __HELPER_FUNCTIONS__
