test_that("Test C++ DAISIE_nonoceanic_spec; output is silent", {
    expect_silent(
        DAISIE_nonoceanic_spec_cpp(
            0.5, # prob_samp
            0.5, # prob_nonend
            100) # mainland_n
    )
})

test_that("Test C++ DAISIE_nonoceanic_spec; output is a list of three vectors", {
    native_spec <- DAISIE_nonoceanic_spec_cpp(
        0.1, # prob_samp
        0.9, # prob_nonend
        1000) # mainland_n
    expect_true(class(native_spec) == "list")
})

test_that("Test C++ DAISIE_nonoceanic_spec; samples native species when probability of sampling is non-zero", {
    native_spec <- DAISIE_nonoceanic_spec_cpp(
        0.1, # prob_samp
        0.9, # prob_nonend
        1000) # mainland_n
    expect_true(is.list(native_spec))
    expect_true(is.vector(native_spec[[1]]))
    expect_true(is.numeric(native_spec[[1]]))
    expect_true(is.vector(native_spec[[2]]))
    expect_true(is.numeric(native_spec[[2]]))
    expect_true(is.vector(native_spec[[3]]))
    expect_true(is.numeric(native_spec[[3]]))
    expect_gt(length(native_spec[[1]]), 0)
    expect_gt(length(native_spec[[2]]), 0)
})

test_that("Test C++ DAISIE_nonoceanic_spec; samples no native species with zero probability of sampling", {
    prob_samp <- 0.0
    prob_nonend <- 0.9
    mainland_n <- 1000
    nonoceanic_sample <- DAISIE_nonoceanic_spec_cpp(
        0.0, # prob_samp
        0.9, # prob_nonend
        1000) # mainland_n
    expect_true(nonoceanic_sample$init_nonend_spec == 0)
    expect_true(nonoceanic_sample$init_end_spec == 0)
    expect_equal(length(nonoceanic_sample$mainland_spec), mainland_n)
})