# DAISIEsims

This repository is the end product of a MSc Biology (Modelling in the Life Sciences track) mini-project of 5 ECTS (three weeks of workload). All stages of development, such as getting familiar with the code, code profiling, and coming up with a strategy fall within these three weeks.  

The overarching goal of the assignment was to improve the speed and efficiency of simulations of the [DAISIE](https://github.com/rsetienne/DAISIE) model. The original model is distributed as an R package, and is thus mostly made up of R code, which is useful for such purposes, but slow. However, R allows for C++ code integration through [Rcpp](https://www.rcpp.org). The Rcpp package provides R functions as well as C++ classes, offering seamless integration of C++ and R. The goal for the three weeks, more specifically, was to make a start with C++ integration of 'slow' functions, while continuously checking whether output stays correct and consistent (through tests).

This repository is not a direct fork of [DAISIE](https://github.com/rsetienne/DAISIE), but rather only includes code that is used by the file that was focussed on during this project; `DAISIE_sim_core_cr.R`. After investigation of the file, we decided to first translate all nested functions from R to C++.  

At time of writing, most functions have been translated, of which most showed improved speed, especially the `update_rates` function. See bottom of this file for a [TODO](#todo) list.

## Refactored functions (R --> C++)

All functions that were translated kept the original name, but `_cpp` was added to it.

- `DAISIE_nonoceanic_spec.cpp` (from `DAISIE_nonoceanic_spec.R`)
- `DAISIE_utils.cpp` (from `DAISIE_utils.R`):
    - `DAISIE_spec_tables`
- `DAISIE_rates.cpp` (from `DAISIE_rates.R`):
    - `update_rates`
    - `island_area`
    - `get_immig_rate`
    - `get_ext_rate`
    - `get_clado_rate`
- `DAISIE_sample_event_cr.cpp` (from `DAISIE_nonoceanic_spec.R`)
    - `DAISIE_sample_event_cr`
- `DAISIE_sim_update_state_cr.cpp` (from `DAISIE_sim_update_state_cr.R`)
    - `DAISIE_sim_update_state_cr`

        Extinction event not fully implemented yet, see [TODO](#todo).

Additionally, we wrote some helper functions to facilitate smooth data transition from Rcpp Sugar objects to C++ objects and vice versa. See [`helper_functions.h`](src/helper_functions.h).

## TODO

- Implement remaining part of extinction event in `DAISIE_sim_update_state_cr.cpp`
- Translate the following functions from R to C++ using Rcpp:
    - `calc_next_timeval`
    - `DAISIE_create_island`
- Once that is done; translate the whole `DAISIE_sim_core_cr.R` file into C++
- As general note; implement logic when trait_pars != NULL  

    As we only focussed on `DAISIE_sim_core_cr.R`, the `trait_pars` argument, when it came across, was always `NULL`. Hence, we did not implement cases where trait_pars was not `NULL` in any of the functions we rewrote.  
