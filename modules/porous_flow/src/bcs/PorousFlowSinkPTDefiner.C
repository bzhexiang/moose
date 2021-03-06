/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/


#include "PorousFlowSinkPTDefiner.h"

template<>
InputParameters validParams<PorousFlowSinkPTDefiner>()
{
  InputParameters params = validParams<PorousFlowSink>();
  return params;
}

PorousFlowSinkPTDefiner::PorousFlowSinkPTDefiner(const InputParameters & parameters) :
    PorousFlowSink(parameters),
    _pp(_involves_fluid ? &getMaterialProperty<std::vector<Real> >("PorousFlow_porepressure_nodal") : NULL),
    _dpp_dvar(_involves_fluid ? &getMaterialProperty<std::vector<std::vector<Real> > >("dPorousFlow_porepressure_nodal_dvar") : NULL),
    _temp(!_involves_fluid ? &getMaterialProperty<Real>("PorousFlow_temperature_nodal") : NULL),
    _dtemp_dvar(!_involves_fluid ? &getMaterialProperty<std::vector<Real> >("dPorousFlow_temperature_nodal_dvar") : NULL)
{
  if (_involves_fluid && (_pp == NULL || _dpp_dvar == NULL))
    mooseError("PorousFlowSink: There is no porepressure Material");
  if (!_involves_fluid && (_temp == NULL || _dtemp_dvar == NULL))
    mooseError("PorousFlowSink: There is no temperature Material");
}

Real
PorousFlowSinkPTDefiner::ptVar()
{
  if (_involves_fluid)
    return (*_pp)[_qp_map[_i]][_ph];
  return (*_temp)[_qp_map[_i]];
}

Real
PorousFlowSinkPTDefiner::dptVar(unsigned pvar)
{
  if (_involves_fluid)
    return (*_dpp_dvar)[_qp_map[_i]][_ph][pvar];
  return (*_dtemp_dvar)[_qp_map[_i]][pvar];
}
