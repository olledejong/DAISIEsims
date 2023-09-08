#include <Rcpp.h>
using namespace Rcpp;
#include <iostream>

// [[Rcpp::export]]
String hello_world() {
    return "Hello World!";
}
