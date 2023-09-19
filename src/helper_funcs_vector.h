#include <iostream>
#include <vector>

#include <Rcpp.h>

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
