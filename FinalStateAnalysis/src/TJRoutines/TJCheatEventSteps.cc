#include "JakobsVBSProcessor.h"

void JakobsVBSProcessor::TJGetInitialCNRecoMasses(EventInfo &info) {

  std::vector<float> initial_cn_reco_masses;

  // Loop over all initial colour neutrals
  // If they come from quarks (only two should!!) write its mass to vector
  // afterwards write to output info.
  streamlog_out(DEBUG) << "In TJGetInitialCNRecoMasses: Got " << nicn() << " total icns." << std::endl;
  for (int i=0; i<nicn(); i++) {
    int initial_cn_parent_pdg = pdg_icn_parent(i);
    if ( initial_cn_parent_pdg == 0 || initial_cn_parent_pdg > 8 ) { continue; }
    streamlog_out(DEBUG) << "In TJGetInitialCNRecoMasses: Got icn with parent PDG " << initial_cn_parent_pdg << std::endl;

    const IntVec& jets_to_icn = jets_of_initial_cn(i);

    TLorentzVector icn_reco_tlv = TLorentzVector( 0, 0, 0, 0 );
    for (int j=0; j<jets_to_icn.size(); j++) {
        int jet_i = jets_to_icn[j];
        TLorentzVector seen_jet_tlv = TLorentzVector( p4seen(jet_i)[1], p4seen(jet_i)[2], p4seen(jet_i)[3], p4seen(jet_i)[0]);
        icn_reco_tlv += seen_jet_tlv;
    }

    initial_cn_reco_masses.push_back( icn_reco_tlv.M() );
  }
  streamlog_out(DEBUG) << "In TJGetInitialCNRecoMasses: Got " << initial_cn_reco_masses.size() << " icns with quark-like parents." << std::endl;

  if (initial_cn_reco_masses.size() != 2) {
    streamlog_out(ERROR) << "ERROR when checking recos to initial cns: NOT 2 QUARK-LIKE ICNs!" << std::endl;
    return;
  }

  info.TJ_eventinfo.TJjets_from_initital_cn.pair1_mass = initial_cn_reco_masses[0];
  info.TJ_eventinfo.TJjets_from_initital_cn.pair2_mass = initial_cn_reco_masses[1];

}
