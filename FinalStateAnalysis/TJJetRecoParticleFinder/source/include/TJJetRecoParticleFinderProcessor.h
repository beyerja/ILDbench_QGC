#ifndef TJJetRecoParticleFinder_h
#define TJJetRecoParticleFinder_h 1

#include "marlin/Processor.h"
#include "lcio.h"
#include <string>
#include <vector>

//LCIO
#include <EVENT/LCCollection.h>
#include <EVENT/ReconstructedParticle.h>

// TrueJet_Parser can link generator level color singlets (quarks,gluons)
// to the jet particles they produced, thereby allowing comparison of
// correct jet with own clustered one.
#include "Adjusted_TrueJet_Parser.h"

using namespace lcio ;
using namespace marlin ;



class TJJetRecoParticleFinder : public Processor , public Adjusted_TrueJet_Parser {

 public:

  virtual Processor*  newProcessor() { return new TJJetRecoParticleFinder ; }


  TJJetRecoParticleFinder() ;

  /** Called at the begin of the job before anything is read.
   * Use to initialize the processor, e.g. book histograms.
   */
  virtual void init() ;

  /** Called for every run.
   */
  virtual void processRunHeader( LCRunHeader* run ) ;

  /** Called for every event - the working horse.
   */
  virtual void processEvent( LCEvent * evt ) ;

  /** For TrueJet_Parser -> see its documentation
  */
  std::string get_recoMCTruthLink(){ return _recoMCTruthLink  ; } ;

  virtual void check( LCEvent * evt ) ;


  /** Called after data processing for clean up.
   */
  virtual void end() ;

 protected:

  int __max_Njets = 20;
  int __Njets = 4;
  int __max_Nparticles = 50;

  /** Input collection name.
   */
  std::string _colAllPFOs {};
  std::string _colMC {};

  std::string _mcpfoRelation {};

  // Input collections for TrueJet
  std::string _recoMCTruthLink {};

  /** Output collection name.
  */
  std::string _RecosFromHadronicJetsName {};

  /** Parameters during Marlin analysis
   */

  int _nRun {};
  int _nEvt {};

  // tj is a pointer to a Trujet_Parser, with the data of this processor object:
  Adjusted_TrueJet_Parser* tj {};
} ;

#endif
