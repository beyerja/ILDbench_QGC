#ifndef VBPAIRFINDING_h
#define VBPAIRFINDING_h 1

#include "StandardIncludes.h"

template<class ParticleClass> class VectorBosonPairFinder {
	/** Class for finding WW or ZZ best candidate within given vector of 
			particles or given vector of candidates
	*/
	
	// These two lines avoid frequent compiler warnings when using -Weffc++
	VectorBosonPairFinder( const VectorBosonPairFinder& ) = delete;
	VectorBosonPairFinder& operator=( const VectorBosonPairFinder& ) = delete;
	
	// Typedefs for better readablitity
	typedef std::vector <ParticleClass*> 	ParticleVec;
	typedef std::vector <float>						FloatVec;
	
	typedef std::pair 	<ParticleClass*, ParticleClass*> 	VB; // = VectorBoson
	typedef std::pair 	<VB, VB> 													VBPair;
	typedef std::vector <VBPair> 													VBPairVec;
	
	typedef std::pair 	<float, float>	MassPair;
	typedef std::vector <MassPair> 			MassPairVec;
	
	public:
		void setMinimizationCriterium( std::string criterium="mass_difference" );
		void setParticleVector( const ParticleVec &particle_vector );
		void setVBPairCandidateVector( const VBPairVec &VBpair_candidates );
		
		void findBestCandidate();
		
		ParticleClass* getBoson1Particle1() const;
		ParticleClass* getBoson1Particle2() const;
		ParticleClass* getBoson2Particle1() const;
		ParticleClass* getBoson2Particle2() const;

		//TODO Separation of minimization criterium allows for different minimization criteria!!!!
		// -> Add SetCriterium(std::string) function! 

	private:

		// Member variables
		std::string			m_minimization_criterium {};
		
		ParticleVec 		m_particle_vector {};
		VBPairVec 			m_VBpair_candidates {};
		VBPairVec 			m_VBpair_candidate_masses {};
		int							m_best_pair_index {};
		
		ParticleClass* 	m_boson1_p1 {};
		ParticleClass*	m_boson1_p2 {};
		ParticleClass*	m_boson2_p1 {};
		ParticleClass*	m_boson2_p2 {};

		// Member functions
		void getAllVBPairCandidates();
		void getMassesToVBPairCandidates();
		void minimizeMassDifference();

		// void fillVPairInformation( 	ParticleVec V1, ParticleVec V2, Observables &observ );
		// void getAllVVTo4ParticleCombinations();
		// void getVVCandidatesMasses( Float2DMatrix &candidates_masses );
		// int findIndexBestPairInVVCandidates();
		
};

#include "VBPairFindingTemplates.tpp"

#endif
