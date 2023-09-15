#include <iostream>
#include <vector>
#include <list>
#include <math.h>
#include <random>

#include <Rcpp.h>

//' Samples what event to happen next
//'
//' @inheritParams default_params_doc
//'
//'@return numeric indicating what event will happen, or a supposed event that
//' would happen in some timesteps of the ontogeny algorithm.
//' \itemize{
//'   \item{[1]: immigration event}
//'   \item{[2]: extinction event}
//'   \item{[3]: cladogenesis event}
//'   \item{[4]: anagenesis event}
//' }
//' @keywords internal
//' @author Pedro Neves
//' @export
// [[Rcpp::export]]
int DAISIE_sample_event_cr_cpp(
    double immig_rate,
    double ext_rate,
    double clado_rate,
    double ana_rate)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> distribution {immig_rate, ext_rate, ana_rate, clado_rate};

    int possible_event = distribution(gen) + 1; // R indexes from 1, not 0 (so 0 should be 1, 1 should be 2, etc)

    return possible_event;
}
