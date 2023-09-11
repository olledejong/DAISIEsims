#include <Rcpp.h>
#include <iostream>
#include <vector>
#include <list>
using namespace Rcpp;

//' @export
// [[Rcpp::export]]
std::list<double> update_rates_cpp() {
  std::list<double> test_list{12,45,8,6};
  return test_list;
}
