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
