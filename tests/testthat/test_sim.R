test_that("test cpp functionality", {

  ## -----------------------------------------------------------------------------
  n_mainland_species <- 1000
  island_age <- 4

  ## -----------------------------------------------------------------------------
  n_replicates <- 1

  ## -----------------------------------------------------------------------------
  set.seed(42)

  ## ----fig.width=7, fig.height=7------------------------------------------------
  clado_rate <- 2.550687345 # cladogenesis rate
  ext_rate <- 2.683454548 # extinction rate
  clade_carr_cap <- Inf # clade-level carrying capacity
  imm_rate <- 0.00933207 # immigration rate
  ana_rate <- 1.010073119 # anagenesis rate

  testthat::expect_silent(
    island_replicates <- DAISIE_sim_cr(
      time = island_age,
      M = n_mainland_species,
      pars = c(clado_rate, ext_rate, clade_carr_cap, imm_rate, ana_rate),
      replicates = n_replicates,
      plot_sims = FALSE,
      verbose = FALSE)
  )
  # DAISIE_plot_sims(island_replicates = island_replicates)
})

# test_that("Clean run should be silent", {

#   set.seed(42)
  
#   n_mainland_species <- 1
#   sim_time <- 10
#   clado_rate <- 1.0
#   ext_rate <- 0.1
#   carr_cap <- 4
#   imm_rate <- 1.0
#   ana_rate <- 1.0

#   area_pars <- create_area_pars(
#     max_area = 1,
#     current_area = 1,
#     proportional_peak_t = 0,
#     total_island_age = 0,
#     sea_level_amplitude = 0,
#     sea_level_frequency = 0,
#     island_gradient_angle = 0)
#   hyper_pars <- create_hyper_pars(d = 0, x = 0)
#   nonoceanic_pars <- c(0, 0)

#   expect_silent(
#     DAISIE_sim_core_cr(
#       time = sim_time,
#       mainland_n = n_mainland_species,
#       pars = c(clado_rate, ext_rate, carr_cap, imm_rate, ana_rate),
#       area_pars = area_pars,
#       hyper_pars = hyper_pars,
#       nonoceanic_pars = nonoceanic_pars
#     )
#   )
# })
