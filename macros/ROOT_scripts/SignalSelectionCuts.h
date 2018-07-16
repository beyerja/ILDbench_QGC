namespace QGCCuts {
  // Define some standard cuts for the aQGC analysis. 
  // To be used in ROOT macros.

  TCut CutVMass( float V_m_min, float V_m_max );
  TCut CutVpT( float V_pT_min );
  TCut CutVAbsCosTheta( float V_cosTheta_max );
  TCut CutVVMass( float VV_m_min, float VV_m_max );
  TCut CutVVpT( float VV_pT_min );
  TCut CutVVET( float VV_ET_min );
  TCut CutMRecoil( float m_recoil_min );
  TCut CutY34( float y_34_min );
  TCut CutJetE( float min_jetE_min );
  TCut CutJetNParticles( float min_jetNparticles_min );
  TCut CutJetNCharged( float min_jetNcharged_min );
  TCut CutLeadETrackAbsCosTheta( float leadEtrack_cosTheta_max );
  TCut CutLeadETrackConeE( float leadEtrack_coneE_min );
  TCut CutIsoLeps();
  
};