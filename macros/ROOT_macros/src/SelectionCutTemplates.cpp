#include "../include/SelectionCutTemplates.h"

/** Disclaimer: Before you read this code make yourself familiar with templates!
*/

//------------------------------------------------------------------------------

template<class ValueType> 
function<bool (ValueType)> Cuts::getMinCutLambda(ValueType min_limit) {
  return [min_limit](ValueType value) { return value > min_limit; };
}

//------------------------------------------------------------------------------

template<class ValueType> 
function<bool (ValueType)> Cuts::getMaxCutLambda(ValueType max_limit) {
  return [max_limit](ValueType value) { return value < max_limit; };
}
  
//------------------------------------------------------------------------------

template<class ValueType> 
function<bool (ValueType)> Cuts::getMinMaxCutLambda(ValueType min_limit, ValueType max_limit) {
  return [min_limit,max_limit] (ValueType value) {
     auto min_cut = Cuts::getMinCutLambda(min_limit);
     auto max_cut = Cuts::getMaxCutLambda(max_limit);
     return min_cut(value) && max_cut(value);
  };
}

//------------------------------------------------------------------------------

function<bool (bool)> Cuts::getBoolCutLambda(bool desired_outcome) {
  return [desired_outcome](bool value) { return value == desired_outcome; };
}

//------------------------------------------------------------------------------