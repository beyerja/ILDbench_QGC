#ifndef JET_CORRECTION_H
#define JET_CORRECTION_H 1

//TODO: Build class for this

double s_tos_p_correction( double p_s, int N_chhad ) {
/*	double p1 = 0.889;
	double p2 = 1.084;
	double p3 = -1.116;
	double p4 = 0.0;
	double p5 = 0.0;
	double p6 = 0.0;
*/
	double p1 = 0.188;
	double p2 = 1.098;
	double p3 = -1.102;
	double p4 = 0.0;
	double p5 = 0.0;
	double p6 = 0.0;


	double offset = p1;
	double p_prop_corr = ( p2 + std::pow(p4, 2) * p_s ) * p_s;
	double N_prop_corr = ( p3 + std::pow(p5, 2) * N_chhad ) * N_chhad;
	double p_N_corr = std::pow( p6, 2 ) * p_s * N_chhad;
	double corrected_p_s = p_prop_corr + N_prop_corr + p_N_corr + offset;

	return corrected_p_s;
}

double s_tos_E_correction( double E_s, int N_chhad, double corrected_p_s ) {
/*	double p1 = 0.901 ;
	double p2 = 1.083;
	double p3 = -1.117;
	double p4 = 0;
	double p5 = 0;
	double p6 = 0;
*/
	double p1 = 0.189 ;
	double p2 = 1.10;
	double p3 = -1.10;
	double p4 = 0;
	double p5 = 0;
	double p6 = 0;


	double offset = p1;
	double E_prop_corr = ( p2 + std::pow(p4, 2) * E_s ) * E_s;
	double N_prop_corr = ( p3 + std::pow(p5, 2) * N_chhad ) * N_chhad;
	double E_N_corr = std::pow( p6, 2 ) * E_s * N_chhad;
	double corrected_E_s = E_prop_corr + N_prop_corr + E_N_corr + offset;

	// NEVER ALLOW M<0 JETS!
	if ( corrected_E_s < corrected_p_s ) {
		corrected_E_s = corrected_p_s;
	} 
	return corrected_E_s;
}


double mass ( double E, double p ) {
	double m2 = std::pow( E, 2 ) - std::pow( p, 2 );
	if ( m2 > 0 ) {
		return std::sqrt( m2 );
	}
	else {
		return std::sqrt( -1. * m2 );
	}
}

#endif
