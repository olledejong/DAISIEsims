#include <iostream>
#include <vector>
#include <list>
#include <math.h>

#include <Rcpp.h>

//' Update internal Gillespie bookeeping objects
//'
//' @param stt_table A species=through-time table.
//' @param total_time Simulated amount of time.
//' @param timeval Current time of simulation.
//' @param mainland_spec A vector with the numeric IDs of the mainland species
//' (i.e. potential colonizers).
//' @param island_spec A matrix with the species on the island (state of the
//' system at each time point).
//'
//' @return A named list with the updated input arguments at time of simulation.
//'
//' @noRd
//' @export
// [[Rcpp::export]]
Rcpp::List DAISIE_spec_tables_cpp(
    int total_time,
    int timeval,
    Rcpp::List nonoceanic_sample,
    int maxspecID)
{
    int init_nonend_spec = nonoceanic_sample["init_nonend_spec"];
    int init_end_spec = nonoceanic_sample["init_end_spec"];
    int mainland_spec = nonoceanic_sample["mainland_spec"];

    // Create stt_table
    Rcpp::IntegerVector stt_table_row = Rcpp::IntegerVector::create(total_time, init_nonend_spec, init_end_spec, 0);
    Rcpp::IntegerMatrix stt_table(1, 4, stt_table_row.begin());
    Rcpp::colnames(stt_table) = Rcpp::CharacterVector::create("Time", "nI", "nA", "nC");

    std::cerr << "\nstt_table: \n" << stt_table;

    return Rcpp::List::create(
        Rcpp::Named("stt_table") = stt_table,
        Rcpp::Named("init_nonend_spec") = init_nonend_spec,
        Rcpp::Named("init_end_spec") = init_end_spec,
        Rcpp::Named("mainland_spec") = mainland_spec,
        Rcpp::Named("island_spec") = Rcpp::IntegerVector::create(), // NOTE empty vector when init_end/nonend spec == 0
        Rcpp::Named("maxspecID") = maxspecID
    );

    // TODO cases below still needs to be implemented (creating island_spec matrix)

    // get amount of rows needed --> create matrix
    // int total_rows = init_nonend_spec + init_end_spec;
    // NOTE matrix with correct amount of rows and 7 columns, initialized with zeros
    // std::vector<std::vector<int>> island_spec(total_rows, std::vector<int>(7, 0)); 

    if (init_nonend_spec != 0) {
        // for (i in seq_along(1:init_nonend_spec)) {
        // island_spec <- rbind(island_spec,
        //                     c(nonoceanic_sample$init_nonend_spec_vec[i],
        //                         nonoceanic_sample$init_nonend_spec_vec[i],
        //                         timeval,
        //                         "I",
        //                         NA,
        //                         NA,
        //                         NA))
        // }
    }
    if (init_end_spec != 0) {
        // for (j in seq_along(1:init_end_spec)) {
        // maxspecID <- maxspecID + 1
        // island_spec <- rbind(island_spec,
        //                     c(maxspecID,
        //                         nonoceanic_sample$init_end_spec_vec[j],
        //                         timeval,
        //                         "A",
        //                         NA,
        //                         NA,
        //                         NA))
        // }
    }

    return Rcpp::List::create(
        Rcpp::Named("stt_table") = stt_table,
        Rcpp::Named("init_nonend_spec") = init_nonend_spec,
        Rcpp::Named("init_end_spec") = init_end_spec,
        Rcpp::Named("mainland_spec") = mainland_spec,
        Rcpp::Named("island_spec") = Rcpp::IntegerVector::create(), // TODO change back to 'island_spec' variable after implementation
        Rcpp::Named("maxspecID") = maxspecID
    );
}
