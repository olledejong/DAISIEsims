#' Translate user-friendly ontogeny codes to numerics
#'
#' @inheritParams default_params_doc
#'
#' @return Numeric, 0 for null-ontogeny, 1 for beta function
#' @noRd
translate_island_ontogeny <- function(island_ontogeny) {

  if (island_ontogeny == "const" || island_ontogeny == 0) {
    island_ontogeny <- 0
  }
  if (island_ontogeny == "beta" || island_ontogeny == 1) {
    island_ontogeny <- 1
  }
  return(island_ontogeny)
}

#' Translate user-friendly sea-level codes to numerics
#'
#' @inheritParams default_params_doc
#'
#' @return Numeric, 0 for null-sea-level, 1 for sine function
#' @noRd
translate_sea_level <- function(sea_level) {

  if (sea_level == "const" || sea_level == 0) {
    sea_level <- 0
  }

  if (sea_level == "sine" || sea_level == 1) {
    sea_level <- 1
  }
  return(sea_level)
}

#' Calculates the species on the island initially when \code{nonoceanic_pars[1]
#' != 0}
#'
#' @param prob_samp probability of a species being sampled
#' from the mainland pool
#' @param prob_nonend probability of a species sampled being non-endemic
#' @param mainland_n number of species in the mainland pool
#'
#' @return A list of non-endemic species, endemic species and the new
#' mainland species pool
#' @noRd
DAISIE_nonoceanic_spec <- function(prob_samp, prob_nonend, mainland_n) {
  testit::assert(prob_samp <= 1)
  testit::assert(prob_samp >= 0)
  testit::assert(prob_nonend <= 1)
  testit::assert(prob_nonend  >= 0)
  testit::assert(length(mainland_n) > 0)
  if (prob_samp != 0) {
    prob_not_samp <- 1 - prob_samp
    prob_nonend <- prob_samp * prob_nonend
    prob_end <- 1 - (prob_not_samp + prob_nonend)
    num_native_spec <- sample(1:3, length(1:mainland_n),
                              replace = TRUE,
                              prob = c(prob_not_samp, prob_nonend, prob_end))
    init_nonend_spec_vec <- sample(1:mainland_n,
                                   length(which(num_native_spec == 2)),
                                   replace = FALSE)
    new_source_pool <- setdiff(1:mainland_n, init_nonend_spec_vec)
    init_end_spec_vec <- sample(new_source_pool,
                                length(which(num_native_spec == 3)),
                                replace = FALSE)
    mainland_spec <- setdiff(1:mainland_n, init_end_spec_vec)
    testit::assert(sum(length(which(num_native_spec == 1)),
                       length(which(num_native_spec == 2)),
                       length(which(num_native_spec == 3)))
                   == sum(mainland_n))
    init_nonend_spec <- length(init_nonend_spec_vec)
    init_end_spec <- length(init_end_spec_vec)
    if (length(mainland_spec) == 0) {
      mainland_spec <- 0
    }
  } else {
    init_nonend_spec <- 0
    init_end_spec <- 0
    init_nonend_spec_vec <- integer(0)
    init_end_spec_vec <- integer(0)
    if(mainland_n != 0){
      mainland_spec <- seq(1, mainland_n, 1)
    } else {
      mainland_spec = c()
    }
  }
  return(list(init_nonend_spec = init_nonend_spec,
              init_end_spec = init_end_spec,
              init_nonend_spec_vec = init_nonend_spec_vec,
              init_end_spec_vec = init_end_spec_vec,
              mainland_spec = mainland_spec))
}

#' Update internal Gillespie bookeeping objects
#'
#' @param stt_table A species=through-time table.
#' @param total_time Simulated amount of time.
#' @param timeval Current time of simulation.
#' @param mainland_spec A vector with the numeric IDs of the mainland species
#' (i.e. potential colonizers).
#' @param island_spec A matrix with the species on the island (state of the
#' system at each time point).
#'
#' @return A named list with the updated input arguments at time of simulation.
#'
#' @noRd
DAISIE_spec_tables <- function(stt_table,
                               total_time,
                               timeval,
                               nonoceanic_sample,
                               island_spec,
                               maxspecID) {
  init_nonend_spec <- nonoceanic_sample$init_nonend_spec
  init_end_spec <- nonoceanic_sample$init_end_spec
  mainland_spec <- nonoceanic_sample$mainland_spec
  stt_table[1, ] <- c(total_time,
                      init_nonend_spec,
                      init_end_spec,
                      0)
  if (init_nonend_spec != 0) {
    for (i in seq_along(1:init_nonend_spec)) {
      island_spec <- rbind(island_spec,
                           c(nonoceanic_sample$init_nonend_spec_vec[i],
                             nonoceanic_sample$init_nonend_spec_vec[i],
                             timeval,
                             "I",
                             NA,
                             NA,
                             NA))
    }
  }
  if (init_end_spec != 0) {
    for (j in seq_along(1:init_end_spec)) {
      maxspecID <- maxspecID + 1
      island_spec <- rbind(island_spec,
                           c(maxspecID,
                             nonoceanic_sample$init_end_spec_vec[j],
                             timeval,
                             "A",
                             NA,
                             NA,
                             NA))
    }
  }
  return(list(stt_table = stt_table,
              init_nonend_spec = init_nonend_spec,
              init_end_spec = init_end_spec,
              mainland_spec = mainland_spec,
              island_spec = island_spec,
              maxspecID = maxspecID))
}
