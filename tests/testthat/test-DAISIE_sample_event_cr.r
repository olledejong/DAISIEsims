# test_that("Test C++ DAISIE_sample_event_cr; picks correct event", {
#     event <- DAISIE_sample_event_cr_cpp(
#         1.0, # immigration rate
#         0.0, # extinction rate
#         0.0, # anagenesis rate
#         0.0 # cladogenesis rate
#     )

#     event2 <- DAISIE_sample_event_cr_cpp(
#         0.0, # immigration rate
#         0.0, # extinction rate
#         0.0, # anagenesis rate
#         1.0 # cladogenesis rate
#     )

#     # events, as described in DAISIE_sim_update_state_cr.R:
#     # 1 = Immigration
#     # 2 = Extinctiion
#     # 3 = Anagenesis
#     # 4 = Cladogenesis

#     expect_true(event == 1)
#     expect_true(event2 == 4)
# })