#include "PDGIDChecks.h"

//-------------------------------------------------------------------------------------------------

bool PDGIDChecks::isNeutrinoID( int pdgID ) {
  if( fabs(pdgID) == 12 || fabs(pdgID) == 14 || fabs(pdgID) == 16 || fabs(pdgID) == 18 ){
      return true;
  } else {
    return false;
  }
}

//-------------------------------------------------------------------------------------------------

bool PDGIDChecks::isChargedLeptonID( int pdgID ) {
  if( fabs(pdgID) > 10 && fabs(pdgID) < 19 && !PDGIDChecks::isNeutrinoID(pdgID) ){
      return true;
  } else {
    return false;
  }
}

//-------------------------------------------------------------------------------------------------

bool PDGIDChecks::isQuarkID( int pdgID ) {
  if( fabs(pdgID) > 0 && fabs(pdgID) < 9 ){
    return true;
  } else {
    return false;
  }
}

//-------------------------------------------------------------------------------------------------

bool PDGIDChecks::isZDecayLike( int p1_pdgID, int p2_pdgID ) {
  // Only consider Z->ffbar decays
  if (  fabs(p1_pdgID) > 0 && fabs(p1_pdgID) < 19 &&  // particle 1 is fermion
        p1_pdgID == -1*p2_pdgID                       // particle 2 is anti-particle of particle 1
      ) {
    return true;    
  } else {
    return false;
  }
}

//-------------------------------------------------------------------------------------------------

bool PDGIDChecks::isZZDecayLike( int Z1p1_pdgID, int Z1p2_pdgID, int Z2p1_pdgID, int Z2p2_pdgID ) {
  if ( PDGIDChecks::isZDecayLike(Z1p1_pdgID,Z1p2_pdgID) && PDGIDChecks::isZDecayLike(Z2p1_pdgID,Z2p2_pdgID)  ) {
    return true;    
  } else {
    return false;
  }
}

//-------------------------------------------------------------------------------------------------

bool PDGIDChecks::isWDecayLike( int p1_pdgID, int p2_pdgID ) {
  // Only consider W->f'f"bar decays
  if (  p1_pdgID * p2_pdgID < 0 && // Must be particle-antiparticle pair
        (
          // If both quarks must be up-type down-type pair
          ( PDGIDChecks::isQuarkID(p1_pdgID) && PDGIDChecks::isQuarkID(p1_pdgID) && (
            ( p1_pdgID % 2 == 0 && p2_pdgID % 2 != 0 ) ||
            ( p1_pdgID % 2 != 0 && p2_pdgID % 2 == 0 ) ) )
          ||
          // If both lepton must be of same generation
          ( ( PDGIDChecks::isNeutrinoID(p1_pdgID) && fabs(p2_pdgID)==fabs(p1_pdgID)-1 ) ||
            ( PDGIDChecks::isNeutrinoID(p2_pdgID) && fabs(p1_pdgID)==fabs(p2_pdgID)-1 ) )
        )
      ) {
    return true;    
  } else {
    return false;
  }          
}
          
//-------------------------------------------------------------------------------------------------
                        
bool PDGIDChecks::isWWDecayLike( int W1p1_pdgID, int W1p2_pdgID, int W2p1_pdgID, int W2p2_pdgID ) {
  if ( PDGIDChecks::isWDecayLike(W1p1_pdgID,W1p2_pdgID) && PDGIDChecks::isWDecayLike(W2p1_pdgID,W2p2_pdgID)  ) {
    return true;    
  } else {
    return false;
  }
}

//-------------------------------------------------------------------------------------------------

