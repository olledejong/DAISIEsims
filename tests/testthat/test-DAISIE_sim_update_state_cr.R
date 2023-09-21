# test_that("DAISIE_sim_update_state C++; immigration event", {

#     island_spec <- c()
#     stt_table <- matrix(ncol = 4)
#     colnames(stt_table) <- c("Time", "nI", "nA", "nC")
#     total_time <- 10
#     timeval <- 0
#     mainland_spec <- c(1:50)
#     mainland_spec <- mainland_spec[-31]
#     init_nonend_spec = 4
#     init_end_spec = 1
#     init_nonend_spec_vec = c(28, 43, 15, 25)
#     init_end_spec_vec = 31

#     maxspecID <- 50
#     spec_tables <- DAISIE_spec_tables_cpp(
#         total_time,
#         timeval,
#         init_nonend_spec,
#         init_end_spec,
#         mainland_spec,
#         init_nonend_spec_vec,
#         init_end_spec_vec,
#         maxspecID
#     )
#     maxspecID <- spec_tables$maxspecID
#     stt_table <- spec_tables$stt_table
#     mainland_spec <- spec_tables$mainland_spec
#     island_spec <- spec_tables$island_spec

#     num_spec <- length(island_spec[, 1])
#     num_immigrants <- length(which(island_spec[, 4] == "I"))

#     hyper_pars <- create_hyper_pars(d = 0, x = 0)
#     area_pars <- create_area_pars(
#         max_area = 1,
#         current_area = 1,
#         proportional_peak_t = 0,
#         total_island_age = 0,
#         sea_level_amplitude = 0,
#         sea_level_frequency = 0,
#         island_gradient_angle = 0
#     )

#     rates <- update_rates_cpp(
#         timeval,
#         total_time,
#         1.0, # immig rate
#         1.0, # anagenesis rate
#         1.0, # caldogenesis rate
#         0.1, # extinction rate
#         hyper_pars, # created by DAISIE_sim_cr
#         area_pars, # created by DAISIE_sim_cr
#         4, # K
#         num_spec,
#         num_immigrants,
#         50, # mainland_n
#         0, # peak
#         0, # island ontogeny
#         0  # sea level
#     )

#     timeval_and_dt <- calc_next_timeval(
#       max_rates = rates,
#       timeval = timeval
#     )
#     timeval <- timeval_and_dt$timeval

#     possible_event <- 1

#     updated_state_cpp <- DAISIE_sim_update_state_cr_cpp(
#         timeval,
#         total_time,
#         possible_event,
#         maxspecID,
#         mainland_spec,
#         stt_table,
#         island_spec
#     )

#     island_spec_cpp <- updated_state_cpp$island_spec
#     maxspecID_cpp <- updated_state_cpp$maxspecID
#     stt_table_cpp <- updated_state_cpp$stt_table

#     updated_state_r <- DAISIE_sim_update_state_cr(
#         timeval = timeval,
#         total_time = total_time,
#         possible_event = possible_event,
#         maxspecID = maxspecID,
#         mainland_spec = mainland_spec,
#         island_spec = island_spec,
#         stt_table = stt_table
#     )

#     island_spec_r <- updated_state_r$island_spec
#     maxspecID_r <- updated_state_r$maxspecID
#     stt_table_r <- updated_state_r$stt_table

#     prmatrix(stt_table_cpp)
#     prmatrix(stt_table_r)
#     cat("\n\n")
#     prmatrix(island_spec_cpp)
#     prmatrix(island_spec_r)

#     # TODO write tests
#     expect_true(TRUE)
# })

# test_that("Test speed of DAISIE_sim_update_state_cr", {
#     island_spec <- c()
#     stt_table <- matrix(ncol = 4)
#     colnames(stt_table) <- c("Time", "nI", "nA", "nC")
#     total_time <- 10
#     timeval <- 0
#     mainland_spec <- c(1:50)
#     mainland_spec <- mainland_spec[-31]
#     init_nonend_spec = 4
#     init_end_spec = 1
#     init_nonend_spec_vec = c(28, 43, 15, 25)
#     init_end_spec_vec = 31

#     maxspecID <- 50
#     spec_tables <- DAISIE_spec_tables_cpp(
#         total_time,
#         timeval,
#         init_nonend_spec,
#         init_end_spec,
#         mainland_spec,
#         init_nonend_spec_vec,
#         init_end_spec_vec,
#         maxspecID
#     )
#     maxspecID <- spec_tables$maxspecID
#     stt_table <- spec_tables$stt_table
#     mainland_spec <- spec_tables$mainland_spec
#     island_spec <- spec_tables$island_spec

#     num_spec <- length(island_spec[, 1])
#     num_immigrants <- length(which(island_spec[, 4] == "I"))

#     hyper_pars <- create_hyper_pars(d = 0, x = 0)
#     area_pars <- create_area_pars(
#         max_area = 1,
#         current_area = 1,
#         proportional_peak_t = 0,
#         total_island_age = 0,
#         sea_level_amplitude = 0,
#         sea_level_frequency = 0,
#         island_gradient_angle = 0
#     )

#     rates <- update_rates_cpp(
#         timeval,
#         total_time,
#         1.0, # immig rate
#         1.0, # anagenesis rate
#         1.0, # caldogenesis rate
#         0.1, # extinction rate
#         hyper_pars, # created by DAISIE_sim_cr
#         area_pars, # created by DAISIE_sim_cr
#         4, # K
#         num_spec,
#         num_immigrants,
#         50, # mainland_n
#         0, # peak
#         0, # island ontogeny
#         0  # sea level
#     )

#     timeval_and_dt <- calc_next_timeval(
#       max_rates = rates,
#       timeval = timeval
#     )
#     timeval <- timeval_and_dt$timeval

#     possible_event <- 1

#     update_states_cpp <- function() {
#         updated_state_cpp <- DAISIE_sim_update_state_cr_cpp(
#             timeval,
#             total_time,
#             possible_event,
#             maxspecID,
#             mainland_spec,
#             stt_table,
#             island_spec
#         )
#     }

#     update_states_r <- function() {
#         updated_state_r <- DAISIE_sim_update_state_cr(
#             timeval = timeval,
#             total_time = total_time,
#             possible_event = possible_event,
#             maxspecID = maxspecID,
#             mainland_spec = mainland_spec,
#             island_spec = island_spec,
#             stt_table = stt_table
#         )
#     }

#     res <- microbenchmark::microbenchmark(update_states_cpp(), update_states_r())

#     cat("\n", "\n")
#     print(res)
#     cat("\n", "\n")

#     expect_true(mean(res$time[res$expr == "update_states_cpp()"]) < mean(res$time[res$expr == "update_states_r()"]))
# })
