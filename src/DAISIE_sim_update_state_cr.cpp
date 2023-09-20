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
    std::vector<std::vector<std::string>> island_spec,
    std::vector<int> stt_table)
{
    std::cerr << "\BLYATTT" << std::endl;
    std::cerr << stt_table[0] << std::endl;
    std::cerr << stt_table[1] << std::endl;
    std::cerr << stt_table[2] << std::endl;
    std::cerr << stt_table[3] << std::endl;

    Rcpp::Function sample2 = Rcpp::Environment::namespace_env("DDD")["sample2"];

    if (possible_event == 1) { // IMMIGRATION
        std::vector<int> colonist = Rcpp::as<std::vector<int>>(sample2(mainland_spec, 1));
        std::string colonist_str = std::to_string(colonist[0]);

        // check if colonist already there -- returns -1 when not present, and row index when present
        int spec_row_index = get_row_index_where_col_equals_str(island_spec, 0, colonist_str);

        if (spec_row_index == -1) {
            // if colonist not yet on island
            // insert row into island_spec matrix
            int current_num_rows = island_spec.size();
            
            island_spec[current_num_rows][0] = colonist_str;
            island_spec[current_num_rows][1] = colonist_str;
            island_spec[current_num_rows][2] = std::to_string(timeval);
            island_spec[current_num_rows][3] = "I";
            island_spec[current_num_rows][4] = ""; // instead of NA
            island_spec[current_num_rows][5] = ""; // instead of NA
            island_spec[current_num_rows][6] = ""; // instead of NA
        
        } else { 
            // already present on island
            // replace row with updated row (new timeval etc)
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
        std::vector<int> immi_row_indexes = get_row_indexes_where_col_equals_str(island_spec, 3, "I");

        std::string anagenesis_str;
        if (immi_row_indexes.size() > 1) {
            // multiple species that immigrated, so we pick one
            std::vector<int> anagenesis = Rcpp::as<std::vector<int>>(sample2(immi_row_indexes, 1));
            anagenesis_str = std::to_string(anagenesis[0]);
        } else { // only one immigrated species on island
            anagenesis_str = std::to_string(immi_row_indexes[0]);
        }
        maxspecID += 1;

        int spec_row_index = get_row_index_where_col_equals_str(island_spec, 0, anagenesis_str);

        island_spec[spec_row_index][3] = "A";
        island_spec[spec_row_index][0] = std::to_string(maxspecID);
        island_spec[spec_row_index][6] = "Immig_parent";

    } else if (possible_event == 4) { // CLADOGENESIS

        std::vector<int> island_spec_ids(island_spec.size());
        std::iota(island_spec_ids.begin(), island_spec_ids.end(), 1);

        // from all species, pick one that will undergo cladogenesis
        std::vector<int> to_split = Rcpp::as<std::vector<int>>(sample2(island_spec_ids, 1));
        std::string to_split_str = std::to_string(to_split[0]);

        // get row index of this species
        int spec_row_index = get_row_index_where_col_equals_str(island_spec, 0, to_split_str);

        // if species that speciates is cladogenetic
        if (island_spec[spec_row_index][3] == "C") {

            // daughter A
            island_spec[spec_row_index][3] = "C";
            island_spec[spec_row_index][0] = std::to_string(maxspecID + 1);
            std::string old_status = island_spec[spec_row_index][4];
            island_spec[spec_row_index][4] = old_status + "A";
            island_spec[spec_row_index][6] = "";

            // daughter B
            // add row to matrix
            std::vector<std::string> new_spec{
                std::to_string(maxspecID + 2),
                island_spec[spec_row_index][1],
                island_spec[spec_row_index][2],
                "C",
                old_status + "B",
                std::to_string(timeval),
                ""
            };
            island_spec.push_back(new_spec);
            
        } else { // species that speciates is not cladogenetic

            // daughter A
            island_spec[spec_row_index][3] = "C";
            island_spec[spec_row_index][0] = std::to_string(maxspecID + 1);
            island_spec[spec_row_index][4] = "A";
            island_spec[spec_row_index][5] = island_spec[spec_row_index][2];
            island_spec[spec_row_index][6] = "";

            // daughter B
            std::vector<std::string> new_spec{
                std::to_string(maxspecID + 2),
                island_spec[spec_row_index][1],
                island_spec[spec_row_index][2],
                "C",
                "B",
                std::to_string(timeval),
                ""
            };
        }
        maxspecID += 2;
    }

    return Rcpp::List::create(
        Rcpp::Named("island_spec") = island_spec,
        Rcpp::Named("maxspecID") = maxspecID,
        Rcpp::Named("stt_table") = stt_table
    );
}