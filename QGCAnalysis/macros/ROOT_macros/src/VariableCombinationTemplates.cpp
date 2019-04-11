#include "../include/VariableCombinationTemplates.h"

/** Disclaimer: Before you read this code make yourself familiar with templates!
*/

//------------------------------------------------------------------------------

template<class ValueType> 
function<ValueType (ValueType, ValueType)> VarComb::getPlusLambda() {
  return [] (ValueType var_1, ValueType var_2) {
     return var_1 + var_2;
  };
}

//------------------------------------------------------------------------------

template<class ValueType> 
function<ValueType (ValueType, ValueType)> VarComb::getMinusLambda() {
  return [] (ValueType var_1, ValueType var_2) {
     return var_1 - var_2;
  };
}

//------------------------------------------------------------------------------

template<class ValueType> 
function<ValueType (ValueType, ValueType)> VarComb::getMeanLambda() {
  return [] (ValueType var_1, ValueType var_2) {
     return ValueType( (var_1 + var_2)/2.0 );
  };
}

//------------------------------------------------------------------------------
