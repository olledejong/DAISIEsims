#include <iostream>
#include <vector>
#include <list>
#include <math.h>
#include <algorithm>

#include <Rcpp.h>

//' Calculates the area at a point in time from a beta function
//'
//' @inheritParams default_params_doc
//'
//' @keywords internal
//'
//' @author Joshua Lambert, Pedro Neves, Shu Xie
//'
//' @return Numeric
double calc_Abeta(
	double proptime,
	double proptime_max,
	int peak,
	double Amax)
{
	double f = proptime_max / (1 - proptime_max);
	double a = f * peak / (1 + f);
	double b = peak / (1 + f);
	double At = Amax * pow(proptime, a) * pow(1 - proptime, b) / (pow(a / (a + b), a) * pow(b / (a + b), b));

	return At;
}

//' Function to describe changes in area through time.
//'
//' @inheritParams default_params_doc
//'
//' @keywords internal
//' @family rate calculations
//' @author Pedro Neves, Joshua Lambert
//' @references
//' Valente, Luis M., Rampal S. Etienne, and Albert B. Phillimore.
//' "The effects of island ontogeny on species diversity and phylogeny."
//' Proceedings of the Royal Society of London B: Biological
//' Sciences 281.1784 (2014): 20133227.
double island_area(
	int timeval,
	int total_time,
	Rcpp::List area_pars,
	int peak,
	int island_ontogeny,
	int sea_level)
{
	int Amax = area_pars["max_area"];

	// Constant ontogeny and sea-level
	if (island_ontogeny == 0 && sea_level == 0)
	{
		if (Amax != 1)
		{
			Rcpp::warning("Constant island area requires a maximum area of 1.");
		}
		return 1;
	}

	// NOTE In all regular (non time-dep) versions of sim_core, island_ontogeny and sea_level always 0, and
	// NOTE therefore the island_area is always set to 1.

	// NOTE In the original R code, the 'proptime' is calculate immediately after extracting of area_pars. In the
	// NOTE very first timestep, in the case of island_ontogeny and sea_level being 0, this results in a divide
	// NOTE by zero error. In R this results in a NaN value, which does not result in exceptions in R since the
	// NOTE 'proptime' value is not used ( island_area stays constant (1) ).

	// NOTE It does, however, result in a 'divide by zero' exception in C++, calling for different handling
	// NOTE For now, we moved the instantiation of 'proptime' to below the above mentioned case ( constant area )

	int Tmax = area_pars["total_island_age"];
	int Acurr = area_pars["current_area"];
	int proptime_max = area_pars["proportional_peak_t"];
	int ampl = area_pars["sea_level_amplitude"];
	int freq = area_pars["sea_level_frequency"];
	int theta = area_pars["island_gradient_angle"];

	double proptime = timeval / Tmax; // Returns NaN in R when both are 0 at the very first time-step
	double proptime_curr = total_time / Tmax;
	double theta_scaled = theta * (M_PI / 180);

	// Beta function ontogeny and constant sea-level
	if (island_ontogeny == 1 && sea_level == 0)
	{
		double At = calc_Abeta(proptime, proptime_max, peak, Amax);
		return At;
	}

	if (island_ontogeny == 0 && sea_level == 1)
	{
		double angular_freq = 2 * M_PI * freq;
		double delta_sl = ampl * cos((proptime_curr - proptime) * angular_freq);
		double r_curr = sqrt((Acurr / M_PI));
		double h_curr = tan(theta_scaled) * r_curr;
		double h_delta = std::max(0.0, h_curr - ampl + delta_sl);
		double At = M_PI * pow(h_delta / tan(theta_scaled), 2);
		return At;
	}

	if (island_ontogeny == 1 && sea_level == 1)
	{
		double A_beta = calc_Abeta(proptime, proptime_max, peak, Amax);

		double angular_freq = 2 * M_PI * freq;
		double delta_sl = ampl * cos((proptime_curr - proptime) * angular_freq);
		double r_curr = sqrt((A_beta / M_PI));
		double h_curr = tan(theta_scaled) * r_curr;
		double h_delta = std::max(0.0, h_curr - ampl + delta_sl);
		double At = M_PI * pow(h_delta / tan(theta_scaled), 2);

		return At;
	}
}

//' Calculate per-capita immigration rate
//'
//' This function is only called directly inside the RHS of the ontogeny
//' likelihood functions. In all other cases \code{\link{get_immig_rate}()} is to
//' be called instead.
//'
//' @inheritParams default_params_doc
//'
//' @return A numeric with the per-capita immigration rate given A(t) and K.
//' @noRd
double get_immig_rate_per_capita(
	double gam,
	int num_spec,
	double K,
	double A)
{
	// NOTE function in R uses pmax, might need tweaking here when traits are implemented
	return std::max(0.0, gam * (1 - (num_spec / (K * A))));
}

//' Calculate immigration rate
//' @description Internal function.
//' Calculates the immigration rate given the current number of
//' species in the system, the carrying capacity
//'
//' @inheritParams default_params_doc
//'
//' @keywords internal
//' @family rate calculations
//' @author Pedro Neves, Joshua Lambert
//' @references Valente, Luis M., Rampal S. Etienne, and Albert B. Phillimore.
//' "The effects of island ontogeny on species diversity and phylogeny."
//' Proceedings of the Royal Society of London B: Biological Sciences 281.1784 (2014): 20133227.
double get_immig_rate(
	double gam,
	int num_spec,
	double K,
	int mainland_n,
	double A = 1.0,
	int trait_pars = 0,
	int island_spec = 0)
{
	if (trait_pars == 0)
	{
		return mainland_n * get_immig_rate_per_capita(
								gam,
								num_spec,
								K,
								A);
	}
	else
	{
		// TODO implement case where trait_pars is not 0
	}
}

//' Function to describe per-capita changes in extinction rate through time
//'
//' This function is only called directly inside the RHS of the ontogeny
//' likelihood functions. In all other cases \code{\link{get_ext_rate}()} is to
//' be called instead.
//'
//' @inheritParams default_params_doc
//'
//' @return Numeric with per capita extinction rate, given A(t), x, and mu0.
//' @noRd
double get_ext_rate_per_capita(
	double mu,
	int x,
	int extcutoff = 1000,
	double A = 1.0)
{
	double ext_rate_pc = std::max(0.0, mu * (pow(A, -x)));
	return std::min(ext_rate_pc, (double)extcutoff);
}

//' Calculate extinction rate
//'
//' @inheritParams default_params_doc
//'
//' @return A numeric, with the extinction rate given the base extinction rate,
//' if present, the hyperparemeter \code{x}, A(t) if time-dependent and traits
//' if running a traint model.
//'
//' @keywords internal
//' @family rate calculations
//' @references Valente, Luis M., Rampal S. Etienne, and Albert B. Phillimore.
//' "The effects of island ontogeny on species diversity and phylogeny."
//' Proceedings of the Royal Society of London B: Biological Sciences 281.1784
//' (2014): 20133227.
//' @author Pedro Neves, Joshua Lambert, Shu Xie
double get_ext_rate(
	double mu,
	int x,
	int num_spec,
	double A = 1.0,
	int extcutoff = 1000,
	int trait_pars = 0,
	int island_spec = 0)
{
	if (trait_pars == 0)
	{
		double ext_rate_pc = num_spec * get_ext_rate_per_capita(
											mu,
											x,
											extcutoff,
											A);
		return std::min(ext_rate_pc, (double)extcutoff);
	}
	else
	{
		// TODO implement case where trait_pars is not 0
	}
}

//' Calculate anagenesis rate
//' @description Internal function.
//' Calculates the anagenesis rate given the current number of
//' immigrant species and the per capita rate.
//'
//' @inheritParams default_params_doc
//'
//' @keywords internal
//' @family rate calculations
//' @author Pedro Neves, Joshua Lambert, Shu Xie
double get_ana_rate(
	double laa,
	int num_immigrants,
	int island_spec = 0,
	int trait_pars = 0)
{
	if (trait_pars == 0)
	{
		return laa * num_immigrants;
	}
	else
	{
		// TODO implement case where trait_pars is not 0
	}
}

//' Calculate per-capita cladogenesis rate
//'
//' This function is only called directly inside the RHS of the ontogeny
//' likelihood functions. In all other cases \code{\link{get_clado_rate}()} is to
//' be called instead.
//'
//' @inheritParams default_params_doc
//'
//' @return Numeric with the per-capita cladogenesis rate given a base
//' cladogenesis rate, K, A and the d hyperparameter.
//' @noRd
double get_clado_rate_per_capita(
	double lac,
	int d,
	int num_spec,
	double K,
	double A = 1.0)
{
	double caldo_rate_pc = lac * (pow(A, d)) * (1 - num_spec / (K * A));
	return std::max(0.0, caldo_rate_pc);
}

//' Calculate cladogenesis rate
//' @description Internal function.
//' Calculates the cladogenesis rate given the current number of
//' species in the system, the carrying capacity and the per capita cladogenesis
//' rate
//'
//' @inheritParams default_params_doc
//'
//' @keywords internal
//' @author Pedro Neves, Joshua Lambert, Shu Xie
double get_clado_rate(
	double lac,
	int d,
	int num_spec,
	double K,
	double A,
	int trait_pars = 0,
	int island_spec = 0)
{
	if (trait_pars == 0)
	{
		return num_spec * get_clado_rate_per_capita(
							  lac,
							  d,
							  num_spec,
							  K,
							  A);
	}
	else
	{
		// TODO implement case where trait_pars is not 0
	}
}

//' Calculates algorithm rates
//' @description Internal function that updates the all the rates and
//' max extinction horizon at time t.
//' @family rate calculations
//'
//' @inheritParams default_params_doc
//'
//' @seealso \code{\link{update_max_rates}()}
//' @keywords internal
//' @return a named list with the updated effective rates.
//' @export
// [[Rcpp::export]]
Rcpp::List update_rates_cpp(
	int timeval,
	int total_time,
	double gam,
	double laa,
	double lac,
	double mu,
	Rcpp::List hyper_pars,
	Rcpp::List area_pars,
	double K,
	int num_spec,
	int num_immigrants,
	int mainland_n,
	int peak = 0,
	int island_ontogeny = 0,
	int sea_level = 0) // NOTE | 'extcutoff' is no longer received here. For now this seems the way to go, might need tweaks later.
{

	double A = island_area(
		timeval,
		total_time,
		area_pars,
		peak,
		island_ontogeny,
		sea_level);

	double immig_rate = get_immig_rate(
		gam,
		num_spec,
		K,
		mainland_n,
		A);

	double ext_rate = get_ext_rate(
		mu,
		hyper_pars["x"],
		num_spec,
		A);

	double ana_rate = get_ana_rate(
		laa,
		num_immigrants);

	double clado_rate = get_clado_rate(
		lac,
		hyper_pars["d"],
		num_spec,
		K,
		A);

	return Rcpp::List::create(
		Rcpp::Named("immig_rate") = immig_rate,
		Rcpp::Named("ext_rate") = ext_rate,
		Rcpp::Named("ana_rate") = ana_rate,
		Rcpp::Named("clado_rate") = clado_rate);
}
