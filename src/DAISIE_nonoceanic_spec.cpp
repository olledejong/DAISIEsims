#include <iostream>
#include <vector>
#include <list>
#include <math.h>

#include <Rcpp.h>

//'Calculates the species on the island initially when \code{nonoceanic_pars[1]
//'!= 0}
//'
//'@param prob_samp probability of a species being sampled
//'from the mainland pool
//'@param prob_nonend probability of a species sampled being non-endemic
//'@param mainland_n number of species in the mainland pool
//'
//'@return A list of non-endemic species, endemic species and the new
//'mainland species pool
//'@noRd
//' @export
// [[Rcpp::export]]
Rcpp::List DAISIE_nonoceanic_spec_cpp(
    int prob_samp,
    int prob_nonend,
    int mainland_n)
{
    // Make accessable for List creation
    int init_nonend_spec = 0;
    int init_end_spec = 0;
    std::vector<int> init_nonend_spec_vec = {0};
    std::vector<int> init_end_spec_vec {0};
    std::vector<int> mainland_spec(mainland_n);

    if (prob_samp != 0)
    {
        // TODO implement when prob of sampling from mainland pool is not zero
        // NOTE always zero when using DAISIE_sim_cr, for now implementation not needed
    }
    else
    {
        if (mainland_n != 0)
        {
            std::iota(mainland_spec.begin(), mainland_spec.end(), 1);
        }
    }

    return Rcpp::List::create(
        Rcpp::Named("init_nonend_spec") = init_nonend_spec,
        Rcpp::Named("init_end_spec") = init_end_spec,
        Rcpp::Named("init_nonend_spec_vec") = init_nonend_spec_vec,
        Rcpp::Named("init_end_spec_vec") = init_end_spec_vec,
        Rcpp::Named("mainland_spec") = mainland_spec
    );
}