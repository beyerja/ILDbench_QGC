#include "aQGCObservablesProcessor.h"

TLorentzVector aQGCObservablesProcessor::getBoostedVector( TLorentzVector &to_be_boosted, TLorentzVector &boost_system ){
  TLorentzVector boosted_tlv = to_be_boosted;
  TVector3 boost_beta_vector = boost_system.BoostVector();
  boosted_tlv.Boost(boost_beta_vector);
  return boosted_tlv;
}