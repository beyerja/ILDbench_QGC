#ifndef PDGIDCHECKS_h
#define PDGIDCHECKS_h 1

#include <math.h>

namespace PDGIDChecks {
  bool isNeutrinoID( int pdgID );
  bool isQuarkID( int pdgID );
  bool isZDecayLike( int p1_pdgID, int p2_pdgID );
  bool isZZDecayLike( int Z1p1_pdgID, int Z1p2_pdgID, int Z2p1_pdgID, int Z2p2_pdgID );
  bool isWDecayLike( int p1_pdgID, int p2_pdgID );
  bool isWWDecayLike( int W1p1_pdgID, int W1p2_pdgID, int W2p1_pdgID, int W2p2_pdgID );
}

#endif