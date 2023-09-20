#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <Rcpp.h>

// TODO move functions that are not used by multiple files

//' Retrieves row indexes where 
int get_row_index_where_col_equals_str(
    std::vector<std::vector<std::string>> mat,
    int col_of_interest,
    std::string query) 
{
    int rowIndex = -1;
    for (int i = 0; i < mat.size(); ++i) {
        std::vector<std::string> row = mat[i];
        if (row[col_of_interest] == query) {
            rowIndex = i;
            break;
        }
    }
    return rowIndex;
}

//' Retrieves row indexes where 
std::vector<int> get_row_indexes_where_col_equals_str(
    std::vector<std::vector<std::string>> mat,
    int col_of_interest,
    std::string query) 
{
    std::vector<int> rowIndexes;
    for (int i = 0; i < mat.size(); ++i) {
        std::vector<std::string> row = mat[i];
        if (row[col_of_interest] == query) {
            rowIndexes.push_back(i);
        }
    }
    return rowIndexes;
}

// function that translates c++ integer vector to R
Rcpp::IntegerVector cppToR_intVec(
    std::vector<int> cppVec)
{
    Rcpp::IntegerVector rVec;
    for (int i = 0; i < cppVec.size(); ++i) {
        rVec.push_back(cppVec[i]);
    }
    return rVec;
}

// function that translates c++ integer matrix to R matrix
Rcpp::IntegerMatrix cppToR_intMat(
    std::vector<std::vector<int>> cppMat)
{
    int numRows = cppMat.size();
    int numCols = cppMat[0].size();

    if (numRows == 0 && numCols == 0)
    {   
        return R_NilValue;
    }

    Rcpp::IntegerMatrix rMat(numRows, numCols);

    for (int i = 0; i < numRows; ++i)
    { // loop rows
        for (int j = 0; j < numCols; ++j)
        {
            rMat(i, j) = cppMat[i][j]; // set cell of matrix to correct value
        }
    }
    return rMat;
}
