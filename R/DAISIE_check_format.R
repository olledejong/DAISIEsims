#' Determines if object rates are rates
#'
#' @inheritParams default_params_doc
#'
#' @return TRUE if object rates is a list of rates
#' @keywords internal
are_rates <- function(rates) {
  # TODO: check if fails on regular 2type model
  if (!all(sapply(rates, is.numeric))) return(FALSE)
  if (!"immig_rate" %in% names(rates)) return(FALSE)
  if (!"ext_rate" %in% names(rates)) return(FALSE)
  if (!"ana_rate" %in% names(rates)) return(FALSE)
  if (!"clado_rate" %in% names(rates)) return(FALSE)
  if (rates$immig_rate < 0.0) return(FALSE)
  if (rates$ext_rate < 0.0) return(FALSE)
  if (rates$ana_rate < 0.0) return(FALSE)
  if (rates$clado_rate < 0.0) return(FALSE)
  if(length(rates) > 4) {
    if (!"immig_rate2" %in% names(rates)) return(FALSE)
    if (!"ext_rate2" %in% names(rates)) return(FALSE)
    if (!"ana_rate2" %in% names(rates)) return(FALSE)
    if (!"clado_rate2" %in% names(rates)) return(FALSE)
    if (!"trans_rate" %in% names(rates)) return(FALSE)
    if (!"trans_rate2" %in% names(rates)) return(FALSE)
    if (rates$immig_rate2 < 0.0) return(FALSE)
    if (rates$ext_rate2 < 0.0) return(FALSE)
    if (rates$ana_rate2 < 0.0) return(FALSE)
    if (rates$clado_rate2 < 0.0) return(FALSE)
    if (rates$trans_rate < 0.0) return(FALSE)
    if (rates$trans_rate2 < 0.0) return(FALSE)
  }

  TRUE
}


#' Test if list has area parameters
#'
#' @inheritParams default_params_doc
#'
#' @return Boolean that indicates if list conforms to expected area parameters
#' as created by \link{create_area_pars}
#' @author Richel J.C Bilderbeek, Joshua Lambert, Pedro Neves
#' @noRd
are_area_pars <- function(area_pars) {
  if (is.null(area_pars) == TRUE) return(TRUE)
  if (class(area_pars) != class(list())) return(FALSE)
  if (!"max_area" %in% names(area_pars)) return(FALSE)
  if (!"current_area" %in% names(area_pars)) return(FALSE)
  if (!"proportional_peak_t" %in% names(area_pars)) return(FALSE)
  if (!"total_island_age" %in% names(area_pars)) return(FALSE)
  if (!"sea_level_amplitude" %in% names(area_pars)) return(FALSE)
  if (!"sea_level_frequency" %in% names(area_pars)) return(FALSE)
  if (!"island_gradient_angle" %in% names(area_pars)) return(FALSE)
  if (area_pars$max_area < 0.0) return(FALSE)
  if (area_pars$proportional_peak_t < 0.0) return(FALSE)
  if (area_pars$proportional_peak_t >= 1.0) return(FALSE)
  if (area_pars$total_island_age < 0.0) return(FALSE)
  if (area_pars$sea_level_amplitude < 0.0) return(FALSE)
  if (area_pars$sea_level_frequency < 0.0) return(FALSE)
  if (area_pars$island_gradient_angle < 0.0) return(FALSE)
  if (area_pars$island_gradient_angle > 90) return(FALSE)
  TRUE
}

#' Test if a list has hyperparameters
#'
#' @inheritParams default_params_doc
#'
#' @return \code{TRUE} if list contains hyperparameters, \code{FALSE} otherwise.
#' @author Pedro Neves, Joshua Lambert
#'
#' @noRd
are_hyper_pars <- function(hyper_pars) {
  if (!is.list(hyper_pars)) return(FALSE)
  if (!is.numeric(unlist(hyper_pars))) return(FALSE)
  if (!"d" %in% names(hyper_pars)) return(FALSE)
  if (!"x" %in% names(hyper_pars)) return(FALSE)
  if (!is.numeric(hyper_pars$x)) return(FALSE)
  if (!is.numeric(hyper_pars$d)) return(FALSE)
  TRUE
}
