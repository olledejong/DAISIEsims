#include <iostream>
#include <vector>

#include <Rcpp.h>

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