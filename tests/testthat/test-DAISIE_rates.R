test_that("Test C++ DAISIE_rates; island area (constant area)", {
    area <- island_area(
        timeval = 2,
        total_time = 5,
        area_pars <- create_area_pars(
            max_area = 1,
            current_area = 1,
            proportional_peak_t = 0,
            total_island_age = 5,
            sea_level_amplitude = 0,
            sea_level_frequency = 0,
            island_gradient_angle = 0
        ),
        island_ontogeny = translate_island_ontogeny("const"),
        sea_level = translate_sea_level("const")
    )
    expect_true(is.numeric(area) && area >= 0)
})

test_that("Test C++ DAISIE_rates; max area too large", {
    expect_warning(
        island_area(
            timeval = 2,
            total_time = 5,
            area_pars = create_area_pars(
                max_area = 10,
                current_area = 1,
                proportional_peak_t = 0.5,
                total_island_age = 5,
                sea_level_amplitude = 5,
                sea_level_frequency = 10,
                island_gradient_angle = 45
            ),
            island_ontogeny = translate_island_ontogeny("const"),
            sea_level = translate_sea_level("const")
        ),
        "Constant island area requires a maximum area of 1."
    )
})

# NOTE not testing ontogeny & sea_level, since it is not implemented yet
