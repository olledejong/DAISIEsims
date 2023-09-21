test_that("Test C++ functionality; clean run should be silent", {
  set.seed(42)

  n_mainland_species <- 1
  sim_time <- 10
  clado_rate <- 1.0
  ext_rate <- 0.1
  carr_cap <- 4
  imm_rate <- 1.0
  ana_rate <- 1.0

  area_pars <- create_area_pars(
    max_area = 1,
    current_area = 1,
    proportional_peak_t = 0,
    total_island_age = 0,
    sea_level_amplitude = 0,
    sea_level_frequency = 0,
    island_gradient_angle = 0
  )
  hyper_pars <- create_hyper_pars(d = 0, x = 0)
  nonoceanic_pars <- c(0, 0)

  expect_silent(
    DAISIE_sim_core_cr(
      time = sim_time,
      mainland_n = n_mainland_species,
      pars = c(clado_rate, ext_rate, carr_cap, imm_rate, ana_rate),
      area_pars = area_pars,
      hyper_pars = hyper_pars,
      nonoceanic_pars = nonoceanic_pars
    )
  )
})

test_that("Test C++ functionality; DAISIE_sim_core output is correct", {
  time <- 1
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
  sim_core <- DAISIE_sim_core_cr(
    time = time,
    mainland_n = mainland_n,
    pars = c(2, 2, 20, 0.1, 1),
    area_pars = area_pars,
    hyper_pars = hyper_pars,
    nonoceanic_pars = nonoceanic_pars
  )
  expect_true(is.matrix(sim_core$stt_table))
  expect_true(sim_core$stt_table[1, 1] == time)
  expect_true(sim_core$stt_table[nrow(sim_core$stt_table), 1] == 0)
  expect_true(is.numeric(sim_core$taxon_list[[1]]$branching_times))
  expect_true(is.numeric(sim_core$taxon_list[[1]]$stac))
  expect_true(is.numeric(sim_core$taxon_list[[1]]$missing_species))
  # NOTE taxon_list length test cannot be used, as we cannot influence the seed on the C++ side.
  # NOTE This is caused by the sampling in DAISIE_sample_event_cr.cpp
  # NOTE For proof that sampling works --> test-DAISIE_sample_event_cr.R
  # expect_true(length(sim_core$taxon_list) == 5)
  expect_true("branching_times" %in% names(sim_core$taxon_list[[1]]))
  expect_true("stac" %in% names(sim_core$taxon_list[[1]]))
  expect_true("missing_species" %in% names(sim_core$taxon_list[[1]]))
})
