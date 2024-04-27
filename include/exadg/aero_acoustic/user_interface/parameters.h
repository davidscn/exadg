/*  ______________________________________________________________________
 *
 *  ExaDG - High-Order Discontinuous Galerkin for the Exa-Scale
 *
 *  Copyright (C) 2023 by the ExaDG authors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *  ______________________________________________________________________
 */

#ifndef EXADG_AERO_ACOUSTIC_USER_INTERFACE_INPUT_PARAMETERS_H_
#define EXADG_AERO_ACOUSTIC_USER_INTERFACE_INPUT_PARAMETERS_H_

// deal.II
#include <deal.II/base/conditional_ostream.h>
#include <deal.II/base/parameter_handler.h>

namespace ExaDG
{
namespace AeroAcoustic
{
enum class FluidToAcousticCouplingStrategy
{
  Undefined,
  ConservativeInterpolation
};

class Parameters
{
public:
  Parameters()
    : density(-1.0),
      source_term_with_convection(false),
      blend_in_source_term(false),
      fluid_to_acoustic_coupling_strategy(FluidToAcousticCouplingStrategy::Undefined),
      compute_acoustic_from_analytical_cfd_solution(false),
      compute_acoustic_from_analytical_source_term(false)
  {
  }

  void
  check() const
  {
    AssertThrow(density >= 0.0, dealii::ExcMessage("Density has to be set."));

    AssertThrow(fluid_to_acoustic_coupling_strategy != FluidToAcousticCouplingStrategy::Undefined,
                dealii::ExcMessage("Coupling strategy has to be set."));

    if(compute_acoustic_from_analytical_cfd_solution)
    {
      AssertThrow(
        not compute_acoustic_from_analytical_source_term,
        dealii::ExcMessage(
          "Only one of the following can be true: compute_acoustic_from_analytical_cfd_solution, compute_acoustic_from_analytical_source_term."));
    }
  }

  void
  print(dealii::ConditionalOStream const & pcout, std::string const & name) const
  {
    pcout << std::endl << name << std::endl << std::endl;
    print_parameter(pcout, "Density", density);
    print_parameter(pcout, "Source term has convective part", source_term_with_convection);
    print_parameter(pcout, "Blend in source term", blend_in_source_term);
    print_parameter(pcout, "Fluid to acoustic coupling", fluid_to_acoustic_coupling_strategy);
    print_parameter(pcout,
                    "Acoustic from analytical CFD solution",
                    compute_acoustic_from_analytical_cfd_solution);
    print_parameter(pcout,
                    "Acoustic from analytical source term",
                    compute_acoustic_from_analytical_source_term);
  }

  void
  add_parameters(dealii::ParameterHandler & prm, std::string const & subsection_name)
  {
    prm.enter_subsection(subsection_name);
    {
      prm.add_parameter(
        "Density", density, "Mean density of underlying fluid.", dealii::Patterns::Double(), true);

      prm.add_parameter("SourceTermWithConvection",
                        source_term_with_convection,
                        "Source term includes convective part.",
                        dealii::Patterns::Bool(),
                        true);

      prm.add_parameter("BlendInSourceTerm",
                        blend_in_source_term,
                        "Blend in the aeroacoustic source term.",
                        dealii::Patterns::Bool(),
                        true);

      prm.add_parameter("FluidToAcousticCouplingStrategy",
                        fluid_to_acoustic_coupling_strategy,
                        "Volume coupling strategy from the fluid to the acoustic field.",
                        Patterns::Enum<FluidToAcousticCouplingStrategy>(),
                        true);

      prm.add_parameter("AcousticFromAnalyticalCFDSolution",
                        compute_acoustic_from_analytical_cfd_solution,
                        "Use analytical CFD solution to compute acoustic.",
                        dealii::Patterns::Bool(),
                        true);

      prm.add_parameter("AcousticFromAnalyticalSourceTerm",
                        compute_acoustic_from_analytical_source_term,
                        "Use analytical source term to compute acoustic.",
                        dealii::Patterns::Bool(),
                        true);
    }
    prm.leave_subsection();
  }

  // mean density of underlying fluid
  double density;

  // The aero-acoustic source term is the material derivative of the
  // pressure. Sometimes, it is sufficient to neglect the convective
  // part of the material derivative.
  bool source_term_with_convection;

  // Blend in aero-acoustic source terms in time or space?
  bool blend_in_source_term;

  // Strategy to couple from fluid to acoustic
  FluidToAcousticCouplingStrategy fluid_to_acoustic_coupling_strategy;

  // In case the analytical CFD solution is known, interpolate it to the
  // CFD triangulation in every time step and compute the acoustic from
  // this as ususal.
  bool compute_acoustic_from_analytical_cfd_solution;

  // In case the aero-acoustic source term is known analytically do not compute a
  // CFD, but interpolate the surce term to the CFD grid and use the given coupling
  // strategy to transfer the source term. If this parameter is activated the
  // cfd solution has to be given and is interpolated to the CFD grid as well.
  // The analytical solution is interpolated as is, i.e. at the point of
  // interpolation there is no check if source_term_with_convection is true. It is
  // the resposibility of the user to make sure the source term includes convection
  // or not.
  bool compute_acoustic_from_analytical_source_term;
};

} // namespace AeroAcoustic
} // namespace ExaDG

#endif /* EXADG_AERO_ACOUSTIC_USER_INTERFACE_INPUT_PARAMETERS_H_ */
