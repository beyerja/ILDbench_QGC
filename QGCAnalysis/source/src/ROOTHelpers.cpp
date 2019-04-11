#include "aQGCObservablesProcessor.h"

TLorentzVector aQGCObservablesProcessor::getBoostedVector( TLorentzVector &to_be_boosted, TLorentzVector &boost_system ){
  TLorentzVector boosted_tlv = to_be_boosted;
  TVector3 boost_beta_vector = -1.0*boost_system.BoostVector();
  boosted_tlv.Boost(boost_beta_vector);
  return boosted_tlv;
}