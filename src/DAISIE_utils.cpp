#include <iostream>
#include <vector>
#include <list>
#include <math.h>

#include <Rcpp.h>

#include "helper_functions.h"

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
    int init_nonend_spec,
    int init_end_spec,
    std::vector<int> mainland_spec,
    std::vector<int> init_nonend_spec_vec,
    std::vector<int> init_end_spec_vec,
    int maxspecID)
{
    // NOTE still doing this usign Rcpp sugar, since working with colnames is challenging, might need changes later
    Rcpp::IntegerVector stt_table_row = Rcpp::IntegerVector::create(total_time, init_nonend_spec, init_end_spec, 0);
    Rcpp::IntegerMatrix stt_table(1, 4, stt_table_row.begin());
    Rcpp::colnames(stt_table) = Rcpp::CharacterVector::create("Time", "nI", "nA", "nC");

    // No rows to add

    if (init_nonend_spec == 0 && init_end_spec == 0) {
        return Rcpp::List::create(
            Rcpp::Named("stt_table") = stt_table,
            Rcpp::Named("init_nonend_spec") = init_nonend_spec,
            Rcpp::Named("init_end_spec") = init_end_spec,
            Rcpp::Named("mainland_spec") = mainland_spec,
            Rcpp::Named("island_spec") = R_NilValue,
            Rcpp::Named("maxspecID") = maxspecID
        );
    }

    // Rows have to be added

    int total_rows = init_nonend_spec + init_end_spec;
    std::vector<std::vector<std::string>> island_spec(total_rows, std::vector<std::string>(7, "")); 
    
    int rows_handled = 0;
    if (init_nonend_spec != 0) { // if we need to add a non-endemic row
        for (int i = 0; i < init_nonend_spec; ++i) {
            island_spec[rows_handled][0] = std::to_string(init_nonend_spec_vec[i]);
            island_spec[rows_handled][1] = std::to_string(init_nonend_spec_vec[i]);
            island_spec[rows_handled][2] = std::to_string(timeval);
            island_spec[rows_handled][3] = "I";
            island_spec[rows_handled][4] = ""; // instead of NA
            island_spec[rows_handled][5] = ""; // instead of NA
            island_spec[rows_handled][6] = ""; // instead of NA
            rows_handled += 1;
        }
    }
    if (init_end_spec != 0) { // if we need to add a endemic row
        for (int i = 0; rows_handled < total_rows; ++i) {
            maxspecID += 1;
            island_spec[rows_handled][0] = std::to_string(maxspecID);
            island_spec[rows_handled][1] = std::to_string(init_end_spec_vec[i]);
            island_spec[rows_handled][2] = std::to_string(timeval);
            island_spec[rows_handled][3] = "A";
            island_spec[rows_handled][4] = ""; // instead of NA
            island_spec[rows_handled][5] = ""; // instead of NA
            island_spec[rows_handled][6] = ""; // instead of NA
            rows_handled += 1;
        }
    }

    return Rcpp::List::create(
        Rcpp::Named("stt_table") = stt_table,
        Rcpp::Named("init_nonend_spec") = init_nonend_spec,
        Rcpp::Named("init_end_spec") = init_end_spec,
        Rcpp::Named("mainland_spec") = mainland_spec,
        Rcpp::Named("island_spec") = cppToR_strMat(island_spec),
        Rcpp::Named("maxspecID") = maxspecID
    );
}