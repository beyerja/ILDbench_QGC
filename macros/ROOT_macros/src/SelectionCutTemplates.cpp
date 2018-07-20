string CutVMass( float cuts [2] ) {
  float V_m_min   = cuts[0];
  float V_m_max   = cuts[1];
  string V1_m_cut = "V1_m>" + to_string(V_m_min) + "&&" + "V1_m<" + to_string(V_m_max);
  string V2_m_cut = "V2_m>" + to_string(V_m_min) + "&&" + "V2_m<" + to_string(V_m_max);
  string V_m_cut = V1_m_cut + "&&" + V2_m_cut;
  return V_m_cut;
}

string CutVpT( float V_pT_min ) {
  string V1_pT_cut = "V1_pT>" + to_string(V_pT_min);
  string V2_pT_cut = "V2_pT>" + to_string(V_pT_min);
  string V_pT_cut = V1_pT_cut + "&&" + V2_pT_cut;
  return V_pT_cut;
}

string CutVAbsCosTheta( float V_cosTheta_max ) {
  string V1_cosTheta_cut = "fabs(V1_cosTheta)<" + to_string(V_cosTheta_max);
  string V2_cosTheta_cut = "fabs(V2_cosTheta)<" + to_string(V_cosTheta_max);
  string V_cosTheta_cut = V1_cosTheta_cut + "&&" + V2_cosTheta_cut;
  return V_cosTheta_cut;
}

string CutVVMass( float cuts [2] ) {
  float VV_m_min  = cuts[0];
  float VV_m_max  = cuts[1];
  string VV_m_cut = "VV_m>" + to_string(VV_m_min) + "&&" + "VV_m<" + to_string(VV_m_max);
  return VV_m_cut;
}

string CutVVpT( float VV_pT_min ) {
  string VV_pT_cut = "VV_pT>" + to_string(VV_pT_min);
  return VV_pT_cut;
}

string CutVVET( float VV_ET_min ) {
  string VV_ET_cut = "VV_ET>" + to_string(VV_ET_min);
  return VV_ET_cut;
}

string CutMRecoil( float m_recoil_min ) {
  string m_recoil_cut = "m_recoil>" + to_string(m_recoil_min);
  return m_recoil_cut;
}

string CutY34( float y_34_min ) {
  string y_34_cut = "y_34>" + to_string(y_34_min);
  return y_34_cut;
}

string CutJetE( float min_jetE_min ) {
  string min_jetE_cut = "min_jetE>" + to_string(min_jetE_min);
  return min_jetE_cut;
}

string CutJetNParticles( float min_jetNparticles_min ) {
  string min_jetNparticles_cut = "min_jetNparticles>" + to_string(min_jetNparticles_min);
  return min_jetNparticles_cut;
}

string CutJetNCharged( float min_jetNcharged_min ) {
  string min_jetNcharged_cut = "min_jetNcharged>" + to_string(min_jetNcharged_min);
  return min_jetNcharged_cut;
}

string CutLeadETrackAbsCosTheta( float leadEtrack_cosTheta_max ) {
  string leadEtrack_cosTheta_cut = "fabs(leadEtrack_cosTheta)<" + to_string(leadEtrack_cosTheta_max);
  return leadEtrack_cosTheta_cut;
}

string CutLeadETrackConeE( float leadEtrack_coneE_min ) {
  string leadEtrack_coneE_cut = "fabs(leadEtrack_coneE)>" + to_string(leadEtrack_coneE_min);
  return leadEtrack_coneE_cut;
}

string CutIsoLeps() { 
  string leadEtrack_coneE_cut = string( "found_isolep==false" );
  return leadEtrack_coneE_cut;
}











//-------------------------------------------------------------------------------------------------










bool PassesCutVMass( float values [2], float cuts [2] ) {
  float V1_m = values[0];
  float V2_m = values[1];
  float V_m_min   = cuts[0];
  float V_m_max   = cuts[1];
  bool passed_V1_m_cut = V1_m > V_m_min && V1_m < V_m_max;
  bool passed_V2_m_cut = V2_m > V_m_min && V2_m < V_m_max;
  return passed_V1_m_cut && passed_V2_m_cut;
}

bool PassesCutVpT( float values [2], float V_pT_min ) {
  float V1_pT = values[0];
  float V2_pT = values[1];
  bool passed_V1_pT_cut = V1_pT > V_pT_min;
  bool passed_V2_pT_cut = V2_pT > V_pT_min;
  return passed_V1_pT_cut && passed_V2_pT_cut;
}

bool PassesCutVAbsCosTheta( float values [2], float V_cosTheta_max ) {
  float V1_cosTheta = values[0];
  float V2_cosTheta = values[1];
  bool passed_V1_cosTheta_cut = fabs(V1_cosTheta) < V_cosTheta_max;
  bool passed_V2_cosTheta_cut = fabs(V2_cosTheta) < V_cosTheta_max;
  return passed_V1_cosTheta_cut && passed_V2_cosTheta_cut;
}

bool PassesCutVVMass( float value, float cuts [2] ) {
  float VV_m_min  = cuts[0];
  float VV_m_max  = cuts[1];
  return value > VV_m_min && value < VV_m_max;
}

bool PassesCutVVpT( float value, float VV_pT_min ) {
  return value > VV_pT_min;
}

bool PassesCutVVET( float value, float VV_ET_min ) {
  return value > VV_ET_min;
}

bool PassesCutMRecoil( float value, float m_recoil_min ) {
  return value > m_recoil_min;
}

bool PassesCutY34( float value, float y_34_min ) {
  return value > y_34_min;
}

bool PassesCutJetE( float value, float min_jetE_min ) {
  return value > min_jetE_min;
}

bool PassesCutJetNParticles( float value, float min_jetNparticles_min ) {
  return value > min_jetNparticles_min;
}

bool PassesCutJetNCharged( float value, float min_jetNcharged_min ) {
  return value > min_jetNcharged_min;
}

bool PassesCutLeadETrackAbsCosTheta( float value, float leadEtrack_cosTheta_max ) {
  return fabs(value) < leadEtrack_cosTheta_max;
}

bool PassesCutLeadETrackConeE( float value, float leadEtrack_coneE_min ) {
  return fabs(value) > leadEtrack_coneE_min;
}

bool PassesCutIsoLeps( bool found_isolep ) { 
  return found_isolep == false;
}