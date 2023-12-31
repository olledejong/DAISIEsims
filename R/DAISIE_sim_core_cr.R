#' Internal function of the DAISIE simulation
#'
#' @rawNamespace importFrom(Rcpp, evalCpp)
#' @inheritParams default_params_doc
#' @export
DAISIE_sim_core_cr <- function(
    time,
    mainland_n,
    pars,
    nonoceanic_pars,
    hyper_pars,
    area_pars
) {

  #### Initialization ####
  timeval <- 0
  total_time <- time

  testit::assert(length(pars) == 5 || length(pars) == 10)

  testit::assert(is.null(area_pars) || are_area_pars(area_pars))
  if (pars[4] == 0 && nonoceanic_pars[1] == 0) {
    stop("Island has no species and the rate of
    colonisation is zero. Island cannot be colonised.")
  }

  nonoceanic_sample <- DAISIE_nonoceanic_spec_cpp(
    nonoceanic_pars[1], # NOTE always zero when using DAISIE_sim_cr
    nonoceanic_pars[2], # NOTE always zero when using DAISIE_sim_cr
    mainland_n)
  maxspecID <- mainland_n

  spec_tables <- DAISIE_spec_tables_cpp(
    total_time,
    timeval,
    nonoceanic_sample$init_nonend_spec, # NOTE pass seperately instead of together
    nonoceanic_sample$init_end_spec,
    nonoceanic_sample$mainland_spec,
    nonoceanic_sample$init_nonend_spec_vec,
    nonoceanic_sample$init_end_spec_vec,
    maxspecID
  )
  maxspecID <- spec_tables$maxspecID
  stt_table <- spec_tables$stt_table
  mainland_spec <- spec_tables$mainland_spec
  island_spec <- spec_tables$island_spec
  lac <- pars[1]
  mu <- pars[2]
  K <- pars[3]
  gam <- pars[4]
  laa <- pars[5]

  num_spec <- length(island_spec[, 1])
  num_immigrants <- length(which(island_spec[, 4] == "I"))

  #### Start Monte Carlo iterations ####
  while (timeval < total_time) {
    rates <- update_rates_cpp(
      timeval,
      total_time,
      gam,
      laa,
      lac,
      mu,
      hyper_pars, # created by DAISIE_sim_cr
      area_pars, # created by DAISIE_sim_cr
      K,
      num_spec,
      num_immigrants,
      mainland_n,
      0, # peak
      0, # island ontogeny
      0  # sea level
    )
    testit::assert(are_rates(rates))

    timeval_and_dt <- calc_next_timeval(
      max_rates = rates,
      timeval = timeval
    )

    timeval <- timeval_and_dt$timeval
    if (timeval <= total_time) {
      rates <- update_rates_cpp(
        timeval,
        total_time,
        gam,
        laa,
        lac,
        mu,
        hyper_pars, # created by DAISIE_sim_cr
        area_pars, # created by DAISIE_sim_cr
        K,
        num_spec,
        num_immigrants,
        mainland_n,
        0, # peak
        0, # island ontogeny
        0  # sea level
      )
      testit::assert(are_rates(rates)) # NOTE are_rates is very slow --> remove when possible

      possible_event <- DAISIE_sample_event_cr_cpp(
        rates$immig_rate,
        rates$ext_rate,
        rates$ana_rate,
        rates$clado_rate
      )

      # TODO remove this if-statement after implementing extinction rate in DAISIE_sim_update_state_cr.cpp
      if (possible_event == 2) {
        possible_event <- 1
      }

      # NOTE if-statement needed to distinguish first loop from others, needs prettier solution
      if (is.null(island_spec)) {
        island_spec <- matrix("x", nrow=1, ncol=2)
      }
      updated_state <- DAISIE_sim_update_state_cr_cpp(
        timeval,
        total_time,
        possible_event,
        maxspecID,
        mainland_spec,
        stt_table,
        island_spec
      )

      island_spec <- updated_state$island_spec
      maxspecID <- updated_state$maxspecID
      stt_table <- updated_state$stt_table
      num_spec <- length(island_spec[, 1])
      num_immigrants <- length(which(island_spec[, 4] == "I"))
    }
  }
  #### Finalize STT ####
  stt_table <- rbind(
    stt_table,
    c(
      0,
      stt_table[nrow(stt_table), 2],
      stt_table[nrow(stt_table), 3],
      stt_table[nrow(stt_table), 4]
    )
  )
  island <- DAISIE_create_island(
    stt_table = stt_table,
    total_time = total_time,
    island_spec = island_spec,
    mainland_n = mainland_n)
  ordered_stt_times <- sort(island$stt_table[, 1], decreasing = TRUE)
  testit::assert(all(ordered_stt_times == island$stt_table[, 1]))
  return(island)
}
