// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// update_rates_cpp
List update_rates_cpp(int timeval, int total_time, double gam, double laa, double lac, double mu, List hyper_pars, List area_pars, double K, int num_spec, int num_immigrants, int mainland_n, int peak, int island_ontogeny, int sea_level);
RcppExport SEXP _DAISIEsims_update_rates_cpp(SEXP timevalSEXP, SEXP total_timeSEXP, SEXP gamSEXP, SEXP laaSEXP, SEXP lacSEXP, SEXP muSEXP, SEXP hyper_parsSEXP, SEXP area_parsSEXP, SEXP KSEXP, SEXP num_specSEXP, SEXP num_immigrantsSEXP, SEXP mainland_nSEXP, SEXP peakSEXP, SEXP island_ontogenySEXP, SEXP sea_levelSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type timeval(timevalSEXP);
    Rcpp::traits::input_parameter< int >::type total_time(total_timeSEXP);
    Rcpp::traits::input_parameter< double >::type gam(gamSEXP);
    Rcpp::traits::input_parameter< double >::type laa(laaSEXP);
    Rcpp::traits::input_parameter< double >::type lac(lacSEXP);
    Rcpp::traits::input_parameter< double >::type mu(muSEXP);
    Rcpp::traits::input_parameter< List >::type hyper_pars(hyper_parsSEXP);
    Rcpp::traits::input_parameter< List >::type area_pars(area_parsSEXP);
    Rcpp::traits::input_parameter< double >::type K(KSEXP);
    Rcpp::traits::input_parameter< int >::type num_spec(num_specSEXP);
    Rcpp::traits::input_parameter< int >::type num_immigrants(num_immigrantsSEXP);
    Rcpp::traits::input_parameter< int >::type mainland_n(mainland_nSEXP);
    Rcpp::traits::input_parameter< int >::type peak(peakSEXP);
    Rcpp::traits::input_parameter< int >::type island_ontogeny(island_ontogenySEXP);
    Rcpp::traits::input_parameter< int >::type sea_level(sea_levelSEXP);
    rcpp_result_gen = Rcpp::wrap(update_rates_cpp(timeval, total_time, gam, laa, lac, mu, hyper_pars, area_pars, K, num_spec, num_immigrants, mainland_n, peak, island_ontogeny, sea_level));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_DAISIEsims_update_rates_cpp", (DL_FUNC) &_DAISIEsims_update_rates_cpp, 15},
    {NULL, NULL, 0}
};

RcppExport void R_init_DAISIEsims(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
