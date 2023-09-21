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
        sum(rates$immig_rate, rates$ext_rate, rates$ana_rate, rates$clado_rate) > 0
    )
    expect_false(
        rates$immig_rate < 0 && rates$ext_rate && rates$ana_rate && rates$clado_rate
    )
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

test_that("Test C++ DAISIE_rates; compare cpp rate outcomes to original rate outcomes", {
    compare_rates <- function(gam = 1, laa = 1, lac = 1, mu = 1) {
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

        rates_cpp <- update_rates_cpp(
            timeval, # timeval
            total_time, # total_time
            gam, # immig rate
            laa, # anagenesis rate
            lac, # caldogenesis rate
            mu, # extinction rate
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
        rates_r <- update_rates(
            timeval = timeval,
            total_time = total_time,
            gam = gam,
            laa = laa,
            lac = lac,
            mu = mu,
            hyper_pars = hyper_pars,
            area_pars = area_pars,
            K = 4,
            num_spec = num_spec,
            num_immigrants = num_immigrants,
            mainland_n = 1,
            extcutoff = NULL,
            island_ontogeny = 0,
            sea_level = 0
        )

        testthat::expect_equal(rates_cpp$immig_rate, rates_r$immig_rate)
        testthat::expect_equal(rates_cpp$ana_rate, rates_r$ana_rate)
        testthat::expect_equal(rates_cpp$clado_rate, rates_r$clado_rate)
        testthat::expect_equal(rates_cpp$ext_rate, rates_r$ext_rate)
    }


    for (incr in seq(0, 1, by = 0.1)) {
        compare_rates(gam = incr)
        compare_rates(laa = incr)
        compare_rates(lac = incr)
        compare_rates(mu = incr)
    }
})

test_that("Microbenchmarking update_rates function", {
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

    update_rates_cpp_version <- function() {
        rates_cpp <- update_rates_cpp(
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
    }

    update_rates_r_version <- function() {
        rates_r <- update_rates(
            timeval = timeval,
            total_time = total_time,
            gam = 1.0, # immig rate
            laa = 1.0, # anagenesis rate
            lac = 1.0, # caldogenesis rate
            mu = 0.1, # extinction rate
            hyper_pars = hyper_pars,
            area_pars = area_pars,
            K = 4,
            num_spec = num_spec,
            num_immigrants = num_immigrants,
            mainland_n = 1,
            extcutoff = NULL,
            island_ontogeny = 0,
            sea_level = 0
        )
    }

    res <- microbenchmark::microbenchmark(update_rates_cpp_version(), update_rates_r_version())
    print(res)
    
    expect_true(mean(res$time[res$expr == "update_rates_cpp_version()"]) < mean(res$time[res$expr == "update_rates_r_version()"]))
})