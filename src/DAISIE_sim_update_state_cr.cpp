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
// [[Rcpp::depends(DDD)]]
// [[Rcpp::export]]
Rcpp::List DAISIE_sim_update_state_cr_cpp(
    double timeval,
    int total_time,
    int possible_event,
    int maxspecID,
    std::vector<int> mainland_spec,
    Rcpp::NumericMatrix stt_table_r,
    Rcpp::StringMatrix island_spec_r)
{
    Rcpp::Function sample2 = Rcpp::Environment::namespace_env("DDD")["sample2"];

    // check if island is empty (if so, a 1-by-1 matrix was created by DAISIE_sim_core_cr)
    // create empty storage vector for island species
    std::vector<std::vector<std::string>> island_spec;
    // if island not empty, translate R matrix into C++ matrix;

    if (island_spec_r.ncol() != 2)
    {
        island_spec = getStrMatrixCpp(island_spec_r);
    }

    // translate stt_table into C++ equivalent (the column names will be added back later)
    std::vector<std::vector<double>> stt_table = getDoubleMatrixCpp(stt_table_r);

    if (possible_event == 1)
    { // IMMIGRATION
        std::vector<int> colonist = Rcpp::as<std::vector<int>>(sample2(mainland_spec, 1));
        std::string colonist_str = std::to_string(colonist[0]);

        // check if colonist already there -- returns -1 when not present, and row index when present
        int spec_row_index = getRowIndexForQuery(island_spec, 0, colonist_str);

        if (spec_row_index == -1)
        {
            // if colonist not yet on island
            // insert row into island_spec matrix
            std::vector<std::string> new_spec{
                colonist_str,
                colonist_str,
                std::to_string(timeval),
                "I",
                "", // instead of NA
                "", // instead of NA
                ""  // instead of NA
            };
            island_spec.push_back(new_spec);
        }
        else
        {
            // already present on island
            // replace row with updated row (new timeval etc)
            island_spec[spec_row_index][0] = colonist_str;
            island_spec[spec_row_index][1] = colonist_str;
            island_spec[spec_row_index][2] = std::to_string(timeval); // NOTE causes loss of precision (6 decimals max)
            island_spec[spec_row_index][3] = "I";
            island_spec[spec_row_index][4] = ""; // instead of NA
            island_spec[spec_row_index][5] = ""; // instead of NA
            island_spec[spec_row_index][6] = ""; // instead of NA
        }
    }
    else if (possible_event == 2)
    { // EXTINCTION

        // create vector containing the ids of all species on island
        std::vector<int> island_spec_ids(island_spec.size());
        std::iota(island_spec_ids.begin(), island_spec_ids.end(), 1);

        // from all species, pick one that will go extinct
        std::vector<int> to_die = Rcpp::as<std::vector<int>>(sample2(island_spec_ids, 1));
        std::string to_die_str = std::to_string(to_die[0]);

        int spec_row_index = getRowIndexForQuery(island_spec, 0, to_die_str);

        std::string species_type = island_spec[spec_row_index][3];

        // remove immigrant or anagenetic
        if (species_type == "I" || species_type == "A")
        {
            island_spec.erase(island_spec.begin() + spec_row_index);
        }

        // remove cladogenetic
        if (species_type == "C")
        {
            // find all indexes where
            std::vector<int> sameInColOne = getRowIndexesForQuery(island_spec, 1, island_spec[spec_row_index][1]);
            std::vector<int> sameInColTwo = getRowIndexesForQuery(island_spec, 2, island_spec[spec_row_index][2]);

            // TODO find overlap between sameInColOne and sameInColTwo
        }

        // TODO implement extinction event
    }
    else if (possible_event == 3)
    { // ANAGENESIS
        // get row indexes of species that immigrated
        std::vector<int> immi_row_indexes = getRowIndexesForQuery(island_spec, 3, "I");

        if (immi_row_indexes.size() != 0)
        {
            int anagenesis_index;
            // we only allow immigrants to undergo anagenesis (so do nothing when immi_row_indexes.size() == 0)
            if (immi_row_indexes.size() > 1)
            {
                // multiple species that immigrated, so we pick one
                std::vector<int> anagenesis = Rcpp::as<std::vector<int>>(sample2(immi_row_indexes, 1));
                anagenesis_index = anagenesis[0];
            }

            if (immi_row_indexes.size() == 1)
            { // only one immigrated species on island
                anagenesis_index = immi_row_indexes[0];
            }
            maxspecID += 1;

            island_spec[anagenesis_index][3] = "A";
            island_spec[anagenesis_index][0] = std::to_string(maxspecID);
            island_spec[anagenesis_index][6] = "Immig_parent";
        }
    }
    else if (possible_event == 4)
    { // CLADOGENESIS

        // get the number of species that are on the island
        std::vector<int> island_spec_ids(island_spec.size());
        // generate a list that holds all possible row indexes (read species)
        std::iota(island_spec_ids.begin(), island_spec_ids.end(), 0);

        // from all species, pick one that will undergo cladogenesis
        std::vector<int> to_split = Rcpp::as<std::vector<int>>(sample2(island_spec_ids, 1));
        int to_split_index = to_split[0];

        // if species that speciates is cladogenetic
        if (island_spec[to_split_index][3] == "C")
        {
            // daughter A - update existing species row
            island_spec[to_split_index][3] = "C";
            island_spec[to_split_index][0] = std::to_string(maxspecID + 1);
            std::string old_status = island_spec[to_split_index][4];
            island_spec[to_split_index][4] = old_status + "A";
            island_spec[to_split_index][6] = "";

            // daughter B - add new species row
            std::vector<std::string> new_spec{
                std::to_string(maxspecID + 2),
                island_spec[to_split_index][1],
                island_spec[to_split_index][2],
                "C",
                old_status + "B",
                std::to_string(timeval),
                ""};
            island_spec.push_back(new_spec);
        }
        else
        { // species that speciates is not cladogenetic
            // daughter A - update existing species row
            island_spec[to_split_index][3] = "C";
            island_spec[to_split_index][0] = std::to_string(maxspecID + 1);
            island_spec[to_split_index][4] = "A";
            island_spec[to_split_index][5] = island_spec[to_split_index][2];
            island_spec[to_split_index][6] = "";

            // daughter B - add new species row
            std::vector<std::string> new_spec{
                std::to_string(maxspecID + 2),
                island_spec[to_split_index][1],
                island_spec[to_split_index][2],
                "C",
                "B",
                std::to_string(timeval),
                ""};
            island_spec.push_back(new_spec);
        }
        maxspecID += 2;
    }
    // add stt_table row for this time-step

    // prepare date for new row
    double time = total_time - timeval;
    double num_Is = getRowIndexesForQuery(island_spec, 3, "I").size();
    double num_As = getRowIndexesForQuery(island_spec, 3, "A").size();
    double num_Cs = getRowIndexesForQuery(island_spec, 3, "C").size();
    std::vector<double> new_stt_table_row{// create row
                                          time,
                                          num_Is,
                                          num_As,
                                          num_Cs};
    stt_table.push_back(new_stt_table_row); // add row

    // translate from C++ matrix to R matrix
    Rcpp::NumericMatrix stt_table_updated = getNumericMatrixR(stt_table);
    colnames(stt_table_updated) = colnames(stt_table_r); // add col names

    // return all data
    return Rcpp::List::create(
        Rcpp::Named("island_spec") = getStrMatrixR(island_spec),
        Rcpp::Named("maxspecID") = maxspecID,
        Rcpp::Named("stt_table") = stt_table_updated);
}