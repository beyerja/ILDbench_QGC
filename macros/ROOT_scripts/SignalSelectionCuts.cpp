TCut CutVMass( float V_m_min, float V_m_max ) {
  string V1_m_cut = "V1_m>" + to_string(V_m_min) + "&&" + "V1_m<" + to_string(V_m_max);
  string V2_m_cut = "V2_m>" + to_string(V_m_min) + "&&" + "V2_m<" + to_string(V_m_max);
  TCut c_V_m = TCut( (V1_m_cut + "&&" + V2_m_cut).c_str() );
  return c_V_m;
}

TCut CutVpT( float V_pT_min ) {
  string V1_pT_cut = "V1_pT>" + to_string(V_pT_min);
  string V2_pT_cut = "V2_pT>" + to_string(V_pT_min);
  TCut c_V_pT = TCut( (V1_pT_cut + "&&" + V2_pT_cut).c_str() );
  return c_V_pT;
}

TCut CutVAbsCosTheta( float V_cosTheta_max ) {
  string V1_cosTheta_cut = "fabs(V1_cosTheta)<" + to_string(V_cosTheta_max);
  string V2_cosTheta_cut = "fabs(V2_cosTheta)<" + to_string(V_cosTheta_max);
  TCut c_V_cosTheta = TCut( (V1_cosTheta_cut + "&&" + V2_cosTheta_cut).c_str() );
  return c_V_cosTheta;
}

TCut CutVVMass( float VV_m_min, float VV_m_max ) {
  string VV_m_cut = "VV_m>" + to_string(VV_m_min) + "&&" + "VV_m<" + to_string(VV_m_max);
  TCut c_VV_m = TCut( (VV_m_cut).c_str() );
  return c_VV_m;
}

TCut CutVVpT( float VV_pT_min ) {
  string VV_pT_cut = "VV_pT>" + to_string(VV_pT_min);
  TCut c_VV_pT = TCut( (VV_pT_cut).c_str() );
  return c_VV_pT;
}

TCut CutVVET( float VV_ET_min ) {
  string VV_ET_cut = "VV_ET>" + to_string(VV_ET_min);
  TCut c_VV_ET = TCut( (VV_ET_cut).c_str() );
  return c_VV_ET;
}

TCut CutMRecoil( float m_recoil_min ) {
  string m_recoil_cut = "m_recoil>" + to_string(m_recoil_min);
  TCut c_m_recoil = TCut( (m_recoil_cut).c_str() ) ;
  return c_m_recoil;
}

TCut CutY34( float y_34_min ) {
  string y_34_cut = "y_34>" + to_string(y_34_min);
  TCut c_y_34 = TCut( (y_34_cut).c_str() ) ;
  return c_y_34;
}

TCut CutJetE( float min_jetE_min ) {
  string min_jetE_cut = "min_jetE>" + to_string(min_jetE_min);
  TCut c_min_jetE = TCut( (min_jetE_cut).c_str() ) ;
  return c_min_jetE;
}

TCut CutJetNParticles( float min_jetNparticles_min ) {
  string min_jetNparticles_cut = "min_jetNparticles>" + to_string(min_jetNparticles_min);
  TCut c_min_jetNparticles = TCut( (min_jetNparticles_cut).c_str() ) ;
  return c_min_jetNparticles;
}

TCut CutJetNCharged( float min_jetNcharged_min ) {
  string min_jetNcharged_cut = "min_jetNcharged>" + to_string(min_jetNcharged_min);
  TCut c_min_jetNcharged = TCut( (min_jetNcharged_cut).c_str() ) ;
  return c_min_jetNcharged;
}

TCut CutLeadETrackAbsCosTheta( float leadEtrack_cosTheta_max ) {
  string leadEtrack_cosTheta_cut = "fabs(leadEtrack_cosTheta)<" + to_string(leadEtrack_cosTheta_max);
  TCut c_leadEtrack_cosTheta = TCut( (leadEtrack_cosTheta_cut).c_str() );
  return c_leadEtrack_cosTheta;
}

TCut CutLeadETrackConeE( float leadEtrack_coneE_min ) {
  string leadEtrack_coneE_cut = "fabs(leadEtrack_coneE)>" + to_string(leadEtrack_coneE_min);
  TCut c_leadEtrack_coneE = TCut( (leadEtrack_coneE_cut).c_str() );
  return c_leadEtrack_coneE;
}

TCut CutIsoLeps() { 
  TCut c_leadEtrack_coneE = TCut( "found_isolep==false" );
  return c_leadEtrack_coneE;
}