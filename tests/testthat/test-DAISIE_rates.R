test_that("Test C++ DAISIE_rates; island area (constant area)", {
    timeval <- 2
    total_time <- 5
    mainland_n <- 100
    set.seed(5)

    area_pars <- create_area_pars(
        max_area = 1,
        current_area = 1,
        proportional_peak_t = 0,
        total_island_age = 0,
        sea_level_amplitude = 0,
        sea_level_frequency = 0,
        island_gradient_angle = 0
    )

    nonoceanic_pars <- c(0, 0)
    hyper_pars <- create_hyper_pars(d = 0, x = 0)

    nonoceanic_sample <- DAISIE_nonoceanic_spec(
        prob_samp = nonoceanic_pars[1],
        prob_nonend = nonoceanic_pars[2],
        mainland_n = mainland_n
    )
    maxspecID <- mainland_n
    island_spec <- c()
    stt_table <- matrix(ncol = 4)
    colnames(stt_table) <- c("Time", "nI", "nA", "nC")
    spec_tables <- DAISIE_spec_tables(
        stt_table,
        total_time,
        timeval,
        nonoceanic_sample,
        island_spec,
        maxspecID
    )

    island_spec <- spec_tables$island_spec
    num_spec <- length(island_spec[, 1])
    num_immigrants <- length(which(island_spec[, 4] == "I"))

    rates <- update_rates_cpp(
        timeval, # timeval
        total_time, # total_time
        1.0, # immig rate
        1.0, # anagenesis rate
        1.0, # caldogenesis rate
        0.1, # extinction rate
        hyper_pars,
        area_pars,
        4, # carrying cap
        num_spec, # current num of species
        num_immigrants, # number of non-endemic species
        1, # number of mainland species
        0, # peak
        0, # island ontogeny
        0 # sea level
    )

    expect_true(
        sum(rates$immig_rate, rates$ext_rate,rates$ana_rate, rates$clado_rate) > 0
    )
    expect_false(
        rates$immig_rate < 0 && rates$ext_rate &&rates$ana_rate && rates$clado_rate
    )

    # TODO Somehow test the correctness of the rates them selfs.
    # NOTE We cannot, however, access the functions that calculate the rates.
    # NOTE We only have the updated rates for a specific time-point.
})

test_that("Test C++ DAISIE_rates; island area (max area too large)", {
    time <- 1
    mainland_n <- 100
    set.seed(5)
    area_pars <- create_area_pars(
        max_area = 10, # <-- too large
        current_area = 1,
        proportional_peak_t = 0,
        total_island_age = 0,
        sea_level_amplitude = 0,
        sea_level_frequency = 0,
        island_gradient_angle = 0
    )
    nonoceanic_pars <- c(0, 0)
    hyper_pars <- create_hyper_pars(d = 0, x = 0)

    expect_warning(
        sim_core <- DAISIE_sim_core_cr(
            time = time,
            mainland_n = mainland_n,
            pars = c(2, 2, 20, 0.1, 1),
            area_pars = area_pars,
            hyper_pars = hyper_pars,
            nonoceanic_pars = nonoceanic_pars
        ),
        "Constant island area requires a maximum area of 1."
    )
})

# NOTE not testing ontogeny & sea_level within island area (not implemented yet)
