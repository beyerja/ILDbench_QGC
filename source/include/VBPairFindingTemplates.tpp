#ifndef VBPAIRFINDING_IMPL_h
#define VBPAIRFINDING_IMPL_h 1

#include "VBPairFindingTemplates.h"

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// General functions

//-------------------------------------------------------------------------------------------------
template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::setMinimizationCriterium( std::string criterium ) {
	m_minimization_criterium = criterium;
}

template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::setParticleVector( const ParticleVec &particle_vector ) {
	m_particle_vector = particle_vector;
}

template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::setVBPairCandidateVector( const VBPairVec &VBpair_candidates ){
	m_VBpair_candidates = VBpair_candidates;
}

//-------------------------------------------------------------------------------------------------

template<class ParticleClass> ParticleClass* VectorBosonPairFinder<ParticleClass>::getBoson1Particle1() const { return m_boson1_p1; }
template<class ParticleClass> ParticleClass* VectorBosonPairFinder<ParticleClass>::getBoson1Particle2() const { return m_boson1_p2; }
template<class ParticleClass> ParticleClass* VectorBosonPairFinder<ParticleClass>::getBoson2Particle1() const { return m_boson2_p1; }
template<class ParticleClass> ParticleClass* VectorBosonPairFinder<ParticleClass>::getBoson2Particle2() const { return m_boson2_p2; }

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Functions for best pair finding

//-------------------------------------------------------------------------------------------------
	
template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::getAllVBPairCandidates() {
	/**	Take a collection of particles and find all possible
	 		VB pair candidates (consisting of 2x2 particles)
	*/
	
	float minimal_mass_diff = std::numeric_limits<float>::infinity();
	
	int n_particles = m_particle_vector.size();
	
	// This loop combination should give you all possible 
	// (particle particle) (particle particle) pair combinations exactly once.
	// ( If you don't believe me test it on paper :P )
	for(int i=0;i<n_particles-3;i++){
		for(int j=i+1;j<n_particles;j++){ 
			for(int k=i+1;k<n_particles-1;k++){
				for(int l=k+1;l<n_particles;l++){
					// Skip combinations that take same quark twice.
					// Duplicate within pair already excluded by starting second loop with i+1 and fourth with k+1.
					if (j==k || j==l) { continue; }
					
					VB pair1 ( m_particle_vector[i], m_particle_vector[j] );
					VB pair2 ( m_particle_vector[k], m_particle_vector[l] );
					VBPair VBpair_candidate ( pair1, pair2 );
					
					m_VBpair_candidates.push_back( VBpair_candidate );
				}
			}
		}
	}
	
}

//-------------------------------------------------------------------------------------------------
template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::getMassesToVBPairCandidates() {
	/** Fill vector with mass pairs to VB pair candidates
			-> Indices of both vectors should be parallel  (alternative for later: Separate class)
	*/

	// for every VB pair candidate
	for (int i_pair=0; i_pair<m_VBpair_candidates.size(); i_pair++) {
		VBPair VBpair_candidate = m_VBpair_candidates[i_pair];
		VB VB1_candidate = VBpair_candidate.first;
		VB VB2_candidate = VBpair_candidate.second;
		
		// Get 4 momenta of all particles in the VB pair candidate
		TLorentzVector VB1_p1_tlv ( VB1_candidate.first->getMomentum(),		VB1_candidate.first->getEnergy() );
		TLorentzVector VB1_p2_tlv ( VB1_candidate.second->getMomentum(), 	VB1_candidate.second->getEnergy() );
		TLorentzVector VB2_p1_tlv ( VB2_candidate.first->getMomentum(), 	VB2_candidate.first->getEnergy() );
		TLorentzVector VB2_p2_tlv ( VB2_candidate.second->getMomentum(), 	VB2_candidate.second->getEnergy() );
		
		// Get masses of the VB candidates
		float VB1_mass = ( VB1_p1_tlv + VB1_p2_tlv ).M();
		float VB2_mass = ( VB2_p1_tlv + VB2_p2_tlv ).M();
		
		MassPair VBpair_candidate_masses ( VB1_mass, VB2_mass );
		
		// Push into member vector for use later
		m_VBpair_candidate_masses.push_back( VBpair_candidate_masses );
  }
}

//-------------------------------------------------------------------------------------------------

template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::minimizeMassDifference() {
	/** Find index of best candidate according to minimal difference between
			masses of the two vector bosons.
	*/
	FloatVec mass_differences {};
	
  for (int i_pair=0; i_pair<m_VBpair_candidate_masses.size(); i_pair++) {
    mass_differences.push_back( fabs( m_VBpair_candidate_masses[i_pair].first - m_VBpair_candidate_masses[i_pair].second ) );
  }
	
	// Find minimal mass difference
	m_best_pair_index = std::distance( 	
												std::begin(mass_differences), 
												std::min_element( std::begin(mass_differences), std::end(mass_differences) ) 
											);
}

//-------------------------------------------------------------------------------------------------

template<class ParticleClass>
void VectorBosonPairFinder<ParticleClass>::findBestCandidate() {
	/** First find all VB pair candidates unless they were explicitly set.
			Then choose function for finding best pair according to given
			minimization criterium.
			The choosen function will then set the index of the best pair, and then
			we set the VB pair particles accordingly here.
	*/
	
	if ( m_VBpair_candidates.empty ) {
		
		if (	m_particle_vector.empty ) {
			streamlog_out(ERROR) << "ERROR in VBPairFinder: No particles were given for pair finding!" << std::endl;
			return;
		}
		
		this->getAllVBPairCandidates();
	}
	
	if (m_minimization_criterium = "mass_difference"){
		this->minimizeMassDifference();
	}
	else {
		streamlog_out(ERROR) << "ERROR in VBPairFinder: Unknown m_minimization_criterium: " << m_minimization_criterium << std::endl;
		return;
	}
	
	m_boson1_p1 = m_VBpair_candidates[m_best_pair_index].first.first;
	m_boson1_p2 = m_VBpair_candidates[m_best_pair_index].first.second;
	m_boson2_p1 = m_VBpair_candidates[m_best_pair_index].second.first;
	m_boson2_p2 = m_VBpair_candidates[m_best_pair_index].second.second;
}

//-------------------------------------------------------------------------------------------------


#endif