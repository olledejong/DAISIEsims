#include <iostream>
#include <vector>
#include <list>
#include <string>

#include <Rcpp.h>

#include "helper_functions.h"

//' Updates state of island given sampled event for a constant rate case.
//'
//' Makes the event happen by updating island species matrix and species IDs.
//' What event happens is determined by the sampling in the algorithm.
//'
//' @inheritParams default_params_doc
//'
//' @return The updated state of the system, which is a list with the
//' \code{island_spec} matrix, an integer \code{maxspecID} with the most recent
//' ID of species and the \code{stt_table}, a matrix with the current species
//' through time table.
//'
//' @keywords internal
//'
//' @seealso \link{DAISIE_sim_core_cr},
//' \link{DAISIE_sim_update_state_cr}
//' @export
// [[Rcpp::export]]
Rcpp::List DAISIE_sim_update_state_cr_cpp(
    double timeval,
    int total_time,
    int possible_event,
    int maxspecID,
    std::vector<int> mainland_spec,
    std::vector<std::vector<std::string>> island_spec, // NOTE comes in as NULL and becomes matrix after 1 simulated event
    std::vector<int> stt_table) 
{
    Rcpp::Function sample2 = Rcpp::Environment::namespace_env("DDD")["sample2"];

    // IMMIGRATION //

    if (possible_event == 1) { 
        std::vector<int> colonist = Rcpp::as<std::vector<int>>(sample2(mainland_spec, 1)); // sample colonist
        std::string colonist_str = std::to_string(colonist[0]);

        int spec_row_index;
        if (island_spec.size() != 0) { // if island not empty

            // check if colonist already there -- returns -1 when not present, and row index when present
            spec_row_index = get_row_index_where_col_equals(island_spec, 0, colonist_str);

        }

        // if colonist not yet on island
        if (spec_row_index == -1) {

            // insert row into island_spec matrix
            int current_num_rows = island_spec.size();
            
            island_spec[current_num_rows][0] = colonist_str;
            island_spec[current_num_rows][1] = colonist_str;
            island_spec[current_num_rows][2] = std::to_string(timeval);
            island_spec[current_num_rows][3] = "I";
            island_spec[current_num_rows][4] = ""; // instead of NA
            island_spec[current_num_rows][5] = ""; // instead of NA
            island_spec[current_num_rows][6] = ""; // instead of NA
        
        } else { // already present on island

            // replace row with updated row (new timeval etc)
            // island_spec[isitthere, ] <- c(colonist, colonist, timeval, "I", NA, NA, NA)
            island_spec[spec_row_index][0] = colonist_str;
            island_spec[spec_row_index][1] = colonist_str;
            island_spec[spec_row_index][2] = std::to_string(timeval);
            island_spec[spec_row_index][3] = "I";
            island_spec[spec_row_index][4] = ""; // instead of NA
            island_spec[spec_row_index][5] = ""; // instead of NA
            island_spec[spec_row_index][6] = ""; // instead of NA

        }
    } else if (possible_event == 2) { // EXTINCTION

        // TODO implement extinction event

    } else if (possible_event == 3) { // ANAGENESIS

        // get row indexes of species that immigrated
        std::vector<int> immi_row_indexes = {2, 5}; 

        if (immi_row_indexes.size() > 1) {
            auto anagenesis = sample2(immi_row_indexes, 1);
        } else {
            int anagenesis = immi_row_indexes[0];
        }
        maxspecID += 1;

        // TODO for every row index in immi_row_indexes:
        // TODO - set species type (index 3 of the species row) to "A"
        // TODO - update species ID by setting index 0 of species row to maxspecID
        // TODO - set index 6 of species row to "Immig_parent"

    } else if (possible_event == 4) { // CLADOGENESIS

        // TODO implement caldogenesis event
        
    }

    return Rcpp::List::create(
        Rcpp::Named("island_spec") = island_spec,
        Rcpp::Named("maxspecID") = maxspecID,
        Rcpp::Named("stt_table") = stt_table
    );
}