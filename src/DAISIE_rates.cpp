#include <iostream>
#include <vector>
#include <list>

#include <Rcpp.h>
using namespace Rcpp;

//' Test function
//' @export
// [[Rcpp::export]]
std::vector<double> update_rates_cpp() {
  std::vector<double> test_list{12,45,8,6};
  return test_list;
}
