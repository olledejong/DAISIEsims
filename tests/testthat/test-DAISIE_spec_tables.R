test_that("DAISIE_spec_tables_cpp; R vs C++", {

    mainland_n <- 100
    total_time <- 1
    timeval <- 0
    init_nonend_spec <- 0
    init_end_spec <- 0
    mainland_spec <- 1:100
    init_nonend_spec_vec <- 0
    init_end_spec_vec <- 0
    maxspecID <- mainland_n

    nonoceanic_sample <- DAISIE_nonoceanic_spec_cpp(
        0, # always zero when using DAISIE_sim_cr
        0, # always zero when using DAISIE_sim_cr
        mainland_n
    )
    island_spec <- c()
    stt_table <- matrix(ncol = 4)
    colnames(stt_table) <- c("Time", "nI", "nA", "nC")

    spec_tables_r <- DAISIE_spec_tables(
        stt_table,
        total_time,
        timeval,
        nonoceanic_sample,
        island_spec,
        maxspecID
    )

    spec_tables_cpp <- DAISIE_spec_tables_cpp(
        total_time,
        timeval,
        init_nonend_spec,
        init_end_spec,
        mainland_spec,
        init_nonend_spec_vec,
        init_end_spec_vec,
        maxspecID
    )

    expect_true(spec_tables_r$stt_table[1] == spec_tables_cpp$stt_table[1])
    expect_true(spec_tables_r$stt_table[2] == spec_tables_cpp$stt_table[2])
    expect_true(spec_tables_r$stt_table[3] == spec_tables_cpp$stt_table[3])
    expect_true(spec_tables_r$stt_table[4] == spec_tables_cpp$stt_table[4])

    expect_true(spec_tables_r$init_nonend_spec == spec_tables_cpp$init_nonend_spec)
    expect_true(spec_tables_r$init_end_spec == spec_tables_cpp$init_end_spec)

    # check lengths
    expect_true(length(spec_tables_r$mainland_spec) == length(spec_tables_cpp$mainland_spec))
    # check first and last item of the vector
    expect_true(spec_tables_r$mainland_spec[1] == spec_tables_cpp$mainland_spec[1])
    expect_true(spec_tables_r$mainland_spec[100] == spec_tables_cpp$mainland_spec[100])

    expect_null(spec_tables_r$island_spec) # both should be NULL
    expect_null(spec_tables_cpp$island_spec)

    expect_true(spec_tables_r$maxspecID == spec_tables_cpp$maxspecID)
})

test_that("DAISIE_spec_tables_cpp produces correct output", {

    # Get test results 

    island_spec <- c()
    stt_table <- matrix(ncol = 4)
    colnames(stt_table) <- c("Time", "nI", "nA", "nC")
    total_time <- 10
    timeval <- 0
    mainland_spec <- c(1:50)
    mainland_spec <- mainland_spec[-31]
    init_nonend_spec = 4
    init_end_spec = 1
    init_nonend_spec_vec = c(28, 43, 15, 25)
    init_end_spec_vec = 31

    maxspecID <- 50
    nonoceanic_tables <- DAISIE_spec_tables_cpp(
        total_time,
        timeval,
        init_nonend_spec,
        init_end_spec,
        mainland_spec,
        init_nonend_spec_vec,
        init_end_spec_vec,
        maxspecID
    )

    # Compare results to expectationns

    expected_stt <- stt_table <- matrix(ncol = 4)
    colnames(expected_stt) <- c("Time", "nI", "nA", "nC")
    expected_stt[1, ] <- c(10, 4, 1, 0)
    expected_mainland_spec <- c(1, 2, 3, 4, 5, 6, 7, 8, 9,
                                10, 11, 12, 13, 14, 15, 16,
                                17, 18, 19, 20, 21, 22, 23,
                                24, 25, 26, 27, 28, 29, 30,
                                32, 33, 34, 35, 36, 37, 38,
                                39, 40, 41, 42, 43, 44, 45,
                                46, 47, 48, 49, 50)
    expected_island_spec <- matrix(ncol = 7, nrow = 5)
    expected_island_spec[1, ] <- c("28", "28", "0", "I", "", "", "")
    expected_island_spec[2, ] <- c("43", "43", "0", "I", "", "", "")
    expected_island_spec[3, ] <- c("15", "15", "0", "I", "", "", "")
    expected_island_spec[4, ] <- c("25", "25", "0", "I", "", "", "")
    expected_island_spec[5, ] <- c("51", "31", "0", "A", "", "", "")

    expect_true(length(nonoceanic_tables) == 6)
    expect_true("stt_table" %in% names(nonoceanic_tables))
    expect_true("init_nonend_spec" %in% names(nonoceanic_tables))
    expect_true("init_end_spec" %in% names(nonoceanic_tables))
    expect_true("mainland_spec" %in% names(nonoceanic_tables))
    expect_true("island_spec" %in% names(nonoceanic_tables))

    expect_equal(nonoceanic_tables$stt_table, expected_stt)
    expect_equal(nonoceanic_tables$init_nonend_spec, 4)
    expect_equal(nonoceanic_tables$init_end_spec, 1)
    expect_equal(nonoceanic_tables$mainland_spec, expected_mainland_spec)
    expect_equal(nonoceanic_tables$island_spec, expected_island_spec)
})