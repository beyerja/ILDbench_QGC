#include "parameter_point_calculation.cc"


class Calculator1DRatio : public ParameterPointCalculator {

	public:

		void do_event_calculation() {
	
			float miss_mass = tree_readout.get_float("miss_mass");
	
		}

};
