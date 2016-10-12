/*
 * InputParametersNavierStokes.h
 *
 *  Created on: May 9, 2016
 *      Author: fehn
 */

#ifndef INCLUDE_INPUTPARAMETERSNAVIERSTOKES_H_
#define INCLUDE_INPUTPARAMETERSNAVIERSTOKES_H_

#include "MultigridInputParameters.h"
#include "deal.II/base/conditional_ostream.h"
#include "PrintFunctions.h"
#include "../include/OutputDataNavierStokes.h"
#include "ErrorCalculationData.h"
#include "LiftAndDragData.h"
#include "PressureDifferenceData.h"


/**************************************************************************************/
/*                                                                                    */
/*                                 MATHEMATICAL MODEL                                 */
/*                                                                                    */
/**************************************************************************************/

/*
 *  ProblemType describes whether a steady or an unsteady problem has to be solved
 */
enum class ProblemType
{
  Undefined,
  Steady,
  Unsteady
};

/*
 *  EquationType describes the physical/mathematical model that has to be solved,
 *  i.e., Stokes equations or Navier-Stokes equations
 */
enum class EquationType
{
  Undefined,
  Stokes,
  NavierStokes
};

/*
 *  Formulation of viscous term: divergence formulation or Laplace formulation
 */
enum class FormulationViscousTerm
{
  Undefined,
  DivergenceFormulation,
  LaplaceFormulation
};

/**************************************************************************************/
/*                                                                                    */
/*                                 PHYSICAL QUANTITIES                                */
/*                                                                                    */
/**************************************************************************************/

// there are currently no enums for this section



/**************************************************************************************/
/*                                                                                    */
/*                             TEMPORAL DISCRETIZATION                                */
/*                                                                                    */
/**************************************************************************************/

/*
 *  Temporal discretization method
 */
enum class TemporalDiscretization
{
  Undefined,
  BDFDualSplittingScheme,
  BDFCoupledSolution
};

/*
 *  the convective term can be either treated explicitly or implicitly
 */
enum class TreatmentOfConvectiveTerm
{
  Undefined,
  Explicit,
  Implicit
};

/*
 * calculation of time step size
 */
enum class TimeStepCalculation
{
  Undefined,
  ConstTimeStepUserSpecified,
  ConstTimeStepCFL,
  AdaptiveTimeStepCFL,
  ConstTimeStepMaxEfficiency
};


/**************************************************************************************/
/*                                                                                    */
/*                              SPATIAL DISCRETIZATION                                */
/*                                                                                    */
/**************************************************************************************/

/*
 *  Spatial discretization method
 */
enum class SpatialDiscretization
{
  Undefined,
  DG,
  DGXWall
};


/*
 *   Interior penalty formulation of viscous term:
 *   SIPG (symmetric IP) or NIPG (non-symmetric IP)
 */
enum class InteriorPenaltyFormulation
{
  Undefined,
  SIPG,
  NIPG
};


/**************************************************************************************/
/*                                                                                    */
/*                        HIGH-ORDER DUAL SPLITTING SCHEME                            */
/*                                                                                    */
/**************************************************************************************/

/*
 *  Preconditioner type for solution of pressure Poisson equation
 */
enum class PreconditionerPressurePoisson
{
  None,
  Jacobi,
  GeometricMultigrid
};

/*
 *  projection type: standard projection (no penalty term),
 *  divergence penalty term, divergence and continuity penalty term (weak projection)
 */
enum class ProjectionType
{
  Undefined,
  NoPenalty,
  DivergencePenalty,
  DivergenceAndContinuityPenalty
};

/*
 *  Type of projection solver
 */
enum class SolverProjection
{
  LU,
  PCG
};

/*
 *  Preconditioner type for solution of projection step
 */
enum class PreconditionerProjection
{
  None,
  Jacobi,
  InverseMassMatrix
};

/*
 *  Solver type for solution of viscous step
 */
enum class SolverViscous
{
  PCG,
  GMRES
};

/*
 *  Preconditioner type for solution of viscous step
 */
enum class PreconditionerViscous
{
  None,
  Jacobi,
  InverseMassMatrix,
  GeometricMultigrid
};

/**************************************************************************************/
/*                                                                                    */
/*                            COUPLED NAVIER-STOKES SOLVER                            */
/*                                                                                    */
/**************************************************************************************/

/*
 *   Solver for linearized Navier-Stokes problem
 */
enum class SolverLinearizedNavierStokes
{
  Undefined,
  GMRES,
  FGMRES
};

/*
 *  Preconditioner type for linearized Navier-Stokes problem
 */
enum class PreconditionerLinearizedNavierStokes
{
  Undefined,
  None,
  BlockDiagonal,
  BlockTriangular,
  BlockTriangularFactorization
};

/*
 *  preconditioner for (1,1) velocity/momentum block in case of block preconditioning
 */
enum class MomentumPreconditioner
{
  Undefined,
  None,
  InverseMassMatrix,
  VelocityDiffusion,
  VelocityConvectionDiffusion
};

/*
 *  solver for momentum preconditioner:
 *  only relevant if MomentumPreconditioner::VelocityConvectionDiffusion
 */
enum class SolverMomentumPreconditioner
{
  Undefined,
  GeometricMultigridVCycle,
  GeometricMultigridGMRES
};

/*
 *  Preconditioner for (2,2) pressure/Schur complement block in case of block preconditioning
 */
enum class SchurComplementPreconditioner
{
  Undefined,
  None,
  InverseMassMatrix,
  LaplaceOperator,
  CahouetChabard,
  Elman,
  PressureConvectionDiffusion
};


/*
 *   Discretization of Laplacian: B: negative divergence operator, B^T gradient operator
 *   classical (BB^T is approximated by negative Laplace operator),
 *   compatible (BM^{-1}B^T)
 */
enum class DiscretizationOfLaplacian
{
  Undefined,
  Classical,
  Compatible
};

/*
 *  solver for Schur complement preconditioner:
 *  only relevant if approximate or exact inversion of Laplace operator
 *  (classical or compatible) is involved in Schur complement preconditioner
 */
enum class SolverSchurComplementPreconditioner
{
  Undefined,
  GeometricMultigridVCycle,
  GeometricMultigridPCG
};



/**************************************************************************************/
/*                                                                                    */
/*                                     TURBULENCE                                     */
/*                                                                                    */
/**************************************************************************************/

// there are currently no enums for this section




/**************************************************************************************/
/*                                                                                    */
/*                               OUTPUT AND POSTPROCESSING                            */
/*                                                                                    */
/**************************************************************************************/

// there are currently no enums for this section

// mass conservation data

struct MassConservationData
{
  MassConservationData()
    :
  calculate_mass_error(false),
  start_time(std::numeric_limits<double>::max()),
  sample_every_time_steps(std::numeric_limits<unsigned int>::max())
  {}

  bool calculate_mass_error;
  double start_time;
  unsigned int sample_every_time_steps;
};

template<int dim>
class InputParametersNavierStokes
{
public:

  // standard constructor that initializes parameters
  InputParametersNavierStokes()
  :
    // MATHEMATICAL MODEL
    problem_type(ProblemType::Undefined),
    equation_type(EquationType::Undefined),
    formulation_viscous_term(FormulationViscousTerm::Undefined),
    right_hand_side(false),

    // PHYSICAL QUANTITIES
    start_time(0.),
    end_time(-1.),
    viscosity(-1.),

    // TEMPORAL DISCRETIZATION
    temporal_discretization(TemporalDiscretization::Undefined),
    treatment_of_convective_term(TreatmentOfConvectiveTerm::Undefined),
    calculation_of_time_step_size(TimeStepCalculation::Undefined),
    max_velocity(-1.),
    cfl(-1.),
    c_eff(-1.),
    time_step_size(-1.),
    max_number_of_time_steps(std::numeric_limits<unsigned int>::max()),
    order_time_integrator(1),
    start_with_low_order(true),

    // SPATIAL DISCRETIZATION
    // spatial discretization method
    spatial_discretization(SpatialDiscretization::Undefined),

    // convective term - currently no parameters

    // viscous term
    IP_formulation_viscous(InteriorPenaltyFormulation::Undefined),
    IP_factor_viscous(1.),

    // gradient term
    gradp_integrated_by_parts(false),
    gradp_use_boundary_data(false),

    // divergence term
    divu_integrated_by_parts(false),
    divu_use_boundary_data(false),

    // special case: pure DBC's
    pure_dirichlet_bc(false),

    // HIGH-ORDER DUAL SPLITTING SCHEME
    // pressure Poisson equation
    IP_factor_pressure(1.),
    preconditioner_pressure_poisson(PreconditionerPressurePoisson::GeometricMultigrid),
    multigrid_data_pressure_poisson(MultigridData()),
    abs_tol_pressure(1.e-20),
    rel_tol_pressure(1.e-12),

    // stability in the limit of small time steps and projection step
    small_time_steps_stability(false),
    use_approach_of_ferrer(false),
    deltat_ref(1.0),

    // projection step
    projection_type(ProjectionType::Undefined),
    penalty_factor_divergence(1.),
    penalty_factor_continuity(1.),
    solver_projection(SolverProjection::PCG),
    preconditioner_projection(PreconditionerProjection::InverseMassMatrix),
    abs_tol_projection(1.e-20),
    rel_tol_projection(1.e-12),

    // viscous step
    solver_viscous(SolverViscous::PCG),
    preconditioner_viscous(PreconditionerViscous::InverseMassMatrix),
    multigrid_data_viscous(MultigridData()),
    abs_tol_viscous(1.e-20),
    rel_tol_viscous(1.e-12),


    // COUPLED NAVIER-STOKES SOLVER
    // nonlinear solver (Newton solver)
    abs_tol_newton(1.e-20),
    rel_tol_newton(1.e-12),
    max_iter_newton(std::numeric_limits<unsigned int>::max()),

    // linear solver
    solver_linearized_navier_stokes(SolverLinearizedNavierStokes::Undefined),
    abs_tol_linear(1.e-20),
    rel_tol_linear(1.e-12),
    max_iter_linear(std::numeric_limits<unsigned int>::max()),

    // preconditioning linear solver
    preconditioner_linearized_navier_stokes(PreconditionerLinearizedNavierStokes::Undefined),
    use_right_preconditioning(true),
    max_n_tmp_vectors(30),

    // preconditioner velocity/momentum block
    momentum_preconditioner(MomentumPreconditioner::Undefined),
    solver_momentum_preconditioner(SolverMomentumPreconditioner::Undefined),
    multigrid_data_momentum_preconditioner(MultigridData()),
    rel_tol_solver_momentum_preconditioner(1.e-12),

    // preconditioner Schur-complement block
    schur_complement_preconditioner(SchurComplementPreconditioner::Undefined),
    discretization_of_laplacian(DiscretizationOfLaplacian::Undefined),
    solver_schur_complement_preconditioner(SolverSchurComplementPreconditioner::Undefined),
    multigrid_data_schur_complement_preconditioner(MultigridData()),
    rel_tol_solver_schur_complement_preconditioner(1.e-12),
  

    // TURBULENCE
    statistics_start_time(std::numeric_limits<double>::max()),
    statistics_every(1),
    cs(0.),
    ml(0.),
    variabletauw(true),
    dtauw(1.),
    max_wdist_xwall(-1.),

 
    // OUTPUT AND POSTPROCESSING

    // print input parameters
    print_input_parameters(false),

    // write output for visualization of results
    output_data(OutputDataNavierStokes()),

    // calculation of error
    error_data(ErrorCalculationData()),

    // output of solver information
    output_solver_info_every_timesteps(1),

    // restart
    write_restart(false),
    restart_interval_time(std::numeric_limits<double>::max()),
    restart_interval_wall_time(std::numeric_limits<double>::max()),
    restart_every_timesteps(std::numeric_limits<unsigned int>::max()),

    // lift and drag
    lift_and_drag_data(LiftAndDragData()),

    // pressure difference
    pressure_difference_data(PressureDifferenceData<dim>()),

    // conservation of mass
    mass_data(MassConservationData())
   {}

  void set_input_parameters();

  void check_input_parameters()
  {
    // MATHEMATICAL MODEL
    AssertThrow(problem_type != ProblemType::Undefined,ExcMessage("parameter must be defined"));
    AssertThrow(equation_type != EquationType::Undefined,ExcMessage("parameter must be defined"));
    AssertThrow(formulation_viscous_term != FormulationViscousTerm::Undefined,ExcMessage("parameter must be defined"));

    // PHYSICAL QUANTITIES
    AssertThrow(end_time > start_time,ExcMessage("parameter end_time must be defined"));
    AssertThrow(viscosity > 0.,ExcMessage("parameter must be defined"));

    // TEMPORAL DISCRETIZATION
    AssertThrow(temporal_discretization != TemporalDiscretization::Undefined,ExcMessage("parameter must be defined"));
    AssertThrow(treatment_of_convective_term != TreatmentOfConvectiveTerm::Undefined,ExcMessage("parameter must be defined"));
    AssertThrow(calculation_of_time_step_size != TimeStepCalculation::Undefined,ExcMessage("parameter must be defined"));

    if(calculation_of_time_step_size != TimeStepCalculation::ConstTimeStepUserSpecified)
    {
      AssertThrow(cfl > 0.,ExcMessage("parameter must be defined"));
      AssertThrow(max_velocity > 0.,ExcMessage("parameter must be defined"));
    }
    if(calculation_of_time_step_size == TimeStepCalculation::ConstTimeStepUserSpecified)
      AssertThrow(time_step_size > 0.,ExcMessage("parameter must be defined"));
    if(calculation_of_time_step_size == TimeStepCalculation::ConstTimeStepMaxEfficiency)
      AssertThrow(c_eff > 0.,ExcMessage("parameter must be defined"));



    // SPATIAL DISCRETIZATION
    AssertThrow(spatial_discretization != SpatialDiscretization::Undefined ,ExcMessage("parameter must be defined"));
    AssertThrow(IP_formulation_viscous != InteriorPenaltyFormulation::Undefined ,ExcMessage("parameter must be defined"));

    // HIGH-ORDER DUAL SPLITTING SCHEME
    if(temporal_discretization == TemporalDiscretization::BDFDualSplittingScheme)
    {
      AssertThrow(projection_type !=ProjectionType::Undefined,ExcMessage("parameter must be defined"));
    }

    // COUPLED NAVIER-STOKES SOLVER
    if(temporal_discretization == TemporalDiscretization::BDFCoupledSolution)
    {
      AssertThrow(preconditioner_linearized_navier_stokes != PreconditionerLinearizedNavierStokes::Undefined,ExcMessage("parameter must be defined"));

      AssertThrow(momentum_preconditioner != MomentumPreconditioner::Undefined,ExcMessage("parameter must be defined"));
      if(momentum_preconditioner == MomentumPreconditioner::VelocityConvectionDiffusion)
        AssertThrow(solver_momentum_preconditioner != SolverMomentumPreconditioner::Undefined,ExcMessage("parameter must be defined"));

      AssertThrow(schur_complement_preconditioner != SchurComplementPreconditioner::Undefined,ExcMessage("parameter must be defined"));
      if(schur_complement_preconditioner == SchurComplementPreconditioner::LaplaceOperator ||
         schur_complement_preconditioner == SchurComplementPreconditioner::CahouetChabard ||
         schur_complement_preconditioner == SchurComplementPreconditioner::Elman ||
         schur_complement_preconditioner == SchurComplementPreconditioner::PressureConvectionDiffusion)
      {
        AssertThrow(discretization_of_laplacian != DiscretizationOfLaplacian::Undefined,ExcMessage("parameter must be defined"));
        AssertThrow(solver_schur_complement_preconditioner != SolverSchurComplementPreconditioner::Undefined,ExcMessage("parameter must be defined"));
      }
    }

    // TURBULENCE

    // OUTPUT AND POSTPROCESSING

  }


  void print(ConditionalOStream &pcout)
  {
    pcout << std::endl
          << "List of input parameters:" << std::endl;

    // MATHEMATICAL MODEL
    print_parameters_mathematical_model(pcout);

    // PHYSICAL QUANTITIES
    print_parameters_physical_quantities(pcout);

    // TEMPORAL DISCRETIZATION
    if(problem_type == ProblemType::Unsteady)
      print_parameters_temporal_discretization(pcout);

    // SPATIAL DISCRETIZATION
    print_parameters_spatial_discretization(pcout);
 
    // HIGH-ORDER DUAL SPLITTING SCHEME 
    if(temporal_discretization == TemporalDiscretization::BDFDualSplittingScheme)
      print_parameters_dual_splitting(pcout);
   
    // COUPLED NAVIER-STOKES SOLVER
    if(  problem_type == ProblemType::Steady ||
        (problem_type == ProblemType::Unsteady &&
         temporal_discretization == TemporalDiscretization::BDFCoupledSolution) )
       print_parameters_coupled_solver(pcout);
   
    // TURBULENCE
    if(false) // TODO
      print_parameters_turbulence(pcout);
  
    // OUTPUT AND POSTPROCESSING
    print_parameters_output_and_postprocessing(pcout);
}


  void print_parameters_mathematical_model(ConditionalOStream &pcout)
  {
     pcout << std::endl
           << "Mathematical model:" << std::endl;

    /*
     *  The definition of string-arrays in this function is somehow redundant with the 
     *  enum declarations but I think C++ does not offer a more elaborate conversion 
     *  from enums to strings
     */
     
     // problem type
     std::string str_problem_type[] = {"Undefined", 
	                                     "Steady",
                                       "Unsteady"};

     print_parameter(pcout, 
                     "Problem type",
                     str_problem_type[(int)problem_type]);

     // equation type
     std::string str_equation_type[] = {"Undefined", 
                                        "Stokes", 
                                        "Navier-Stokes"};

     print_parameter(pcout, 
                     "Equation type", 
                     str_equation_type[(int)equation_type]);
     
     // formulatiion of viscous term
     std::string str_form_viscous_term[] = {"Undefined", 
                                            "Divergence formulation", 
                                            "Laplace formulation"};

     print_parameter(pcout, 
                     "Formulation of viscous term", 
                     str_form_viscous_term[(int)formulation_viscous_term]);
   
     // right hand side
     print_parameter(pcout,"Right-hand side",right_hand_side);
  }
  

  void print_parameters_physical_quantities(ConditionalOStream &pcout)
  {
    pcout << std::endl
          << "Physical quantities:" << std::endl;

    // start and end time
    if(problem_type == ProblemType::Unsteady)
    {
      print_parameter(pcout,"Start time",start_time);
      print_parameter(pcout,"End time",end_time);
    }
    
    // viscosity
    print_parameter(pcout,"Viscosity",viscosity);
  }  

  void print_parameters_temporal_discretization(ConditionalOStream &pcout)
  {
    pcout << std::endl
          << "Temporal discretization:" << std::endl;

    /*
     *  The definition of string-arrays in this function is somehow redundant with the 
     *  enum declarations but I think C++ does not offer a more elaborate conversion 
     *  from enums to strings
     */  
 
    // temporal discretization scheme
    std::string str_temporal_discretization[] = { "Undefined",
                                                  "BDF dual splitting scheme",
                                                  "BDF coupled solution" };
    print_parameter(pcout,
                    "Temporal discretization method",
                    str_temporal_discretization[(int)temporal_discretization]);
    
    // treatment of convective term
    std::string str_conv_term[] = { "Undefined",
                                    "Explicit",
                                    "Implicit" };
       
    print_parameter(pcout,
                    "Treatment of convective term",
                    str_conv_term[(int)treatment_of_convective_term]);

    // calculation of time step size
    std::string str_calc_time_step[] = { "Undefined",
                                         "Constant time step (user specified)",
                                         "Constant time step (CFL condition)",
                                         "Adaptive time step (CFL condition)",
                                         "Constant time step (max. efficiency)"};

    print_parameter(pcout,
                    "Calculation of time step size",
                    str_calc_time_step[(int)calculation_of_time_step_size]);


    // here we do not print quantities such as max_velocity, cfl, time_step_size
    // because this is done by the time integration scheme (or the functions that 
    // calculate the time step size)

    // maximum number of time steps
    print_parameter(pcout,
                    "Maximum number of time steps",
                    max_number_of_time_steps);

    // order of time integration scheme
    print_parameter(pcout,
                    "Order of time integration scheme",
                    order_time_integrator);

    // start time integrator with high or low order method
    print_parameter(pcout,
                    "Start with low order method",
                    start_with_low_order);
  }

  void print_parameters_spatial_discretization(ConditionalOStream &pcout)
  {
    pcout << std::endl
          << "Spatial discretization:" << std::endl;

    /*
     *  The definition of string-arrays in this function is somehow redundant with the 
     *  enum declarations but I think C++ does not offer a more elaborate conversion 
     *  from enums to strings
     */  

    // Spatial discretization method
    std::string str_spatial_discret[] = { "Undefined",
                                          "DG",
			                                    "DG-XWALL" };

    print_parameter(pcout,
                    "Spatial discretization method",
                    str_spatial_discret[(int)spatial_discretization]);


    // interior penalty formulation of viscous term
    std::string str_IP_form_visc[] = { "Undefined",
                                       "SIPG",
                                       "NIPG" };

    print_parameter(pcout,
                    "Viscous term - IP formulation",
                    str_IP_form_visc[(int)IP_formulation_viscous]);

    print_parameter(pcout,
                    "Viscous term - IP factor", IP_factor_viscous);

    // pressure gradient term
    print_parameter(pcout,
                    "Grad(p) - integration by parts",
                    gradp_integrated_by_parts);

    print_parameter(pcout,
                    "Grad(p) - use boundary data",
                    gradp_use_boundary_data);

    // divergence term
    print_parameter(pcout,
                    "Div(u) . integration by parts",
                    divu_integrated_by_parts);

    print_parameter(pcout,
                    "Div(u) - use boundary data",
                    divu_use_boundary_data);

    // special case pure DBC's
    print_parameter(pcout,
                    "Pure Dirichlet BC's",
                    pure_dirichlet_bc);
  } 

  void print_parameters_dual_splitting(ConditionalOStream &pcout)
  {
    pcout << std::endl
          << "High-order dual splitting scheme:" << std::endl;

    /*
     *  The definition of string-arrays in this function is somehow redundant with the 
     *  enum declarations but I think C++ does not offer a more elaborate conversion 
     *  from enums to strings
     */

    // pressure Poisson equation
    pcout << "  Pressure Poisson equation (PPE):" << std::endl;

    print_parameter(pcout, "IP factor PPE",IP_factor_pressure); 
  
    std::string str_precon_ppe[] = { "None",
                                     "Jacobi",
                                     "GeometricMultigrid" };

    print_parameter(pcout,
                    "Preconditioner PPE",
                    str_precon_ppe[(int)preconditioner_pressure_poisson]);
    
    if(preconditioner_pressure_poisson == PreconditionerPressurePoisson::GeometricMultigrid)
    {   
      std::string str_multigrid_coarse_ppe[] = { "Chebyshev smoother",
                                                 "PCG - no preconditioner",
                                                 "PCG - Jacobi preconditioner",
                                                 "GMRES - No preconditioner",
                                                 "GMRES - Jacobi preconditioner"};
  
      print_parameter(pcout,"Smoother polynomial degree",multigrid_data_pressure_poisson.smoother_poly_degree);
      print_parameter(pcout,"Smoothing range",multigrid_data_pressure_poisson.smoother_smoothing_range);
      print_parameter(pcout,
                      "Multigrid coarse grid solver",
                      str_multigrid_coarse_ppe[(int)multigrid_data_pressure_poisson.coarse_solver]);
    }

    print_parameter(pcout,"Absolute solver tolerance",abs_tol_pressure);

    print_parameter(pcout,"Relative solver tolerance",rel_tol_pressure);

    // small time steps stability
    pcout << std::endl << "  Small time steps stability:" << std::endl;

    print_parameter(pcout,"STS stability approach",small_time_steps_stability);

    print_parameter(pcout,"Approach of Ferrer et al.",use_approach_of_ferrer);
    if(use_approach_of_ferrer == true)
      print_parameter(pcout,"Reference time step size (Ferrer)",deltat_ref);

    // projection step
    pcout << std::endl << "  Projection step:" << std::endl;

    std::string str_proj_type[] = { "Undefined",
                                    "No penalty",
                                    "Divergence penalty",
                                    "Divergence and continuity penalty" };

    print_parameter(pcout,
                    "Projection type",
                    str_proj_type[(int)projection_type]);

    if(projection_type == ProjectionType::DivergencePenalty ||
       projection_type == ProjectionType::DivergenceAndContinuityPenalty)
      print_parameter(pcout,
                      "Penalty factor divergence",
                      penalty_factor_divergence);

    if(projection_type == ProjectionType::DivergenceAndContinuityPenalty)
      print_parameter(pcout,
                      "Penalty factor continuity",
                      penalty_factor_continuity);
   
    if(projection_type == ProjectionType::DivergencePenalty ||
       projection_type == ProjectionType::DivergenceAndContinuityPenalty)
    {  
      std::string str_solver_proj[] = { "LU",
                                        "PCG" };

      print_parameter(pcout,
                      "Solver projection step",
                      str_solver_proj[(int)solver_projection]);

      std::string str_precon_proj[] = { "None",
                                        "Jacobi",
                                        "InverseMassMatrix" };
  
      print_parameter(pcout,
                      "Preconditioner projection step",
                      str_precon_proj[(int)preconditioner_projection]);

      print_parameter(pcout,"Absolute solver tolerance", abs_tol_projection);
      print_parameter(pcout,"Relative solver tolerance", rel_tol_projection);
    }

    // Viscous step
    pcout << std::endl << "  Viscous step:" << std::endl;

    std::string str_solver_viscous[] = { "PCG",
                                         "GMRES" };

    print_parameter(pcout,
                    "Solver viscous step",
                    str_solver_viscous[(int)solver_viscous]);

    std::string str_precon_viscous[] = { "None",
                                         "Jacobi",
                                         "InverseMassMatrix",
                                         "GeometricMultigrid" };

    print_parameter(pcout,
                    "Preconditioner viscous step",
                    str_precon_viscous[(int)preconditioner_viscous]);

    if(preconditioner_viscous == PreconditionerViscous::GeometricMultigrid)
    {  
      std::string str_multigrid_coarse_viscous[] = { "Chebyshev smoother",
                                                     "PCG - no preconditioner",
                                                     "PCG - Jacobi preconditioner",
                                                     "GMRES - No preconditioner",
                                                     "GMRES - Jacobi preconditioner"};
  
      print_parameter(pcout,"Smoother polynomial degree",multigrid_data_viscous.smoother_poly_degree);
      print_parameter(pcout,"Smoothing range",multigrid_data_viscous.smoother_smoothing_range);
      print_parameter(pcout,
                      "Multigrid coarse grid solver",
                      str_multigrid_coarse_viscous[(int)multigrid_data_viscous.coarse_solver]);
    }

    print_parameter(pcout,"Absolute solver tolerance", abs_tol_viscous);
    print_parameter(pcout,"Relative solver tolerance", rel_tol_viscous);
  } 


  void print_parameters_coupled_solver(ConditionalOStream &pcout)
  {
    pcout << std::endl
          << "Coupled Navier-Stokes solver:" << std::endl;

    /*
     *  The definition of string-arrays in this function is somehow redundant with the 
     *  enum declarations but I think C++ does not offer a more elaborate conversion 
     *  from enums to strings
     */

    // Newton solver

    // if a nonlinear problem has to be solved
    if(equation_type == EquationType::NavierStokes &&
       (problem_type == ProblemType::Steady ||
        treatment_of_convective_term == TreatmentOfConvectiveTerm::Implicit) )
   {
      pcout << "Newton solver" << std::endl;
    
      print_parameter(pcout,"Absolute solver tolerance",abs_tol_newton);
      print_parameter(pcout,"Relative solver tolerance",rel_tol_newton);
      print_parameter(pcout,"Maximum number of iterations",max_iter_newton);

      pcout << std::endl;
    }

    // Solver linearized problem    
    pcout << "Linear solver" << std::endl;

    std::string str_solver_linearized[] = { "Undefined",
                                            "GMRES",
                                            "FGMRES" };

    print_parameter(pcout,
                    "Solver for linear(ized) problem",
                    str_solver_linearized[(int)solver_linearized_navier_stokes]);

    print_parameter(pcout,"Absolute solver tolerance",abs_tol_linear);
    print_parameter(pcout,"Relative solver tolerance",rel_tol_linear);
    print_parameter(pcout,"Maximum number of iterations",max_iter_linear);

    std::string str_precon_linear[] = { "Undefined",
                                        "None",
                                        "BlockDiagonal",
                                        "BlockTriangular",
                                        "BlockTriangularFactorization" };

    print_parameter(pcout,
                    "Preconditioner linear(ized) problem",
                    str_precon_linear[(int)preconditioner_linearized_navier_stokes]);

    print_parameter(pcout,"Right preconditioning",use_right_preconditioning);

    if(solver_linearized_navier_stokes == SolverLinearizedNavierStokes::GMRES)
      print_parameter(pcout,"Max number of vectors before restart",max_n_tmp_vectors);

    // preconditioner momentum block
    std::string str_momentum_precon[] = { "Undefined",
                                          "None",
                                          "InverseMassMatrix",
                                          "VelocityConvectionDiffusion"};

    print_parameter(pcout,
                    "Preconditioner momentum block",
                    str_momentum_precon[(int)momentum_preconditioner]);                                 
 
    if(momentum_preconditioner == MomentumPreconditioner::VelocityConvectionDiffusion)
    {
      std::string str_solver_momentum_precon[] = { "Undefined",
                                                   "GeometricMultigridVCycle",
                                                   "GeometricMultigridGMRES" };

      print_parameter(pcout,
                      "Solver momentum preconditioner",
                      str_solver_momentum_precon[(int)solver_momentum_preconditioner]);

      if(solver_momentum_preconditioner == SolverMomentumPreconditioner::GeometricMultigridGMRES)
      {
        print_parameter(pcout,
                        "Relative solver tolerance",
                        rel_tol_solver_momentum_preconditioner);
      }

      print_parameter(pcout,"Smoother polynomial degree",multigrid_data_momentum_preconditioner.smoother_poly_degree);
      print_parameter(pcout,"Smoothing range",multigrid_data_momentum_preconditioner.smoother_smoothing_range);

      std::string str_multigrid_coarse_solver[] = { "Chebyshev smoother",
                                                    "PCG - no preconditioner",
                                                    "PCG - Jacobi preconditioner",
                                                    "GMRES - No preconditioner",
                                                    "GMRES - Jacobi preconditioner"};

      print_parameter(pcout,
                      "Multigrid coarse grid solver",
                      str_multigrid_coarse_solver[(int)multigrid_data_momentum_preconditioner.coarse_solver]);
    }

    // preconditioner Schur-complement block
    std::string str_schur_precon[] = { "Undefined",
                                       "None",
                                       "InverseMassMatrix",
                                       "LaplaceOperator",
                                       "CahouetChabard",
                                       "Elman",
                                       "PressureConvectionDiffusion" };

    print_parameter(pcout,
                    "Schur-complement preconditioner",
                    str_schur_precon[(int)schur_complement_preconditioner]);

    if(schur_complement_preconditioner == SchurComplementPreconditioner::LaplaceOperator ||
       schur_complement_preconditioner == SchurComplementPreconditioner::CahouetChabard ||
       schur_complement_preconditioner == SchurComplementPreconditioner::Elman ||
       schur_complement_preconditioner == SchurComplementPreconditioner::PressureConvectionDiffusion)
    {
      std::string str_discret_laplacian[] = { "Undefined",
                                              "Classical",
                                              "Compatible" };
      print_parameter(pcout,
                      "Discretization of Laplacian",
                      str_discret_laplacian[(int)discretization_of_laplacian]);

      std::string str_solver_schur_precon[] = { "Undefined",
                                                "GeometricMultigridVCycle",
                                                "GeometricMultigridPCG" };

      print_parameter(pcout,
                      "Solver Schur-complement preconditioner",
                      str_solver_schur_precon[(int)solver_schur_complement_preconditioner]);

      if(solver_schur_complement_preconditioner == 
           SolverSchurComplementPreconditioner::GeometricMultigridPCG)
      {
        print_parameter(pcout,
                        "Relative solver tolerance",
                        rel_tol_solver_schur_complement_preconditioner);
      }

      print_parameter(pcout,"Smoother polynomial degree",multigrid_data_schur_complement_preconditioner.smoother_poly_degree);
      print_parameter(pcout,"Smoothing range",multigrid_data_schur_complement_preconditioner.smoother_smoothing_range);
      
      std::string str_multigrid_coarse_solver[] = { "Chebyshev smoother",
                                                    "PCG - no preconditioner",
                                                    "PCG - Jacobi preconditioner",
                                                    "GMRES - No preconditioner",
                                                    "GMRES - Jacobi preconditioner"};
      print_parameter(pcout,
                      "Multigrid coarse grid solver",
                      str_multigrid_coarse_solver[(int)multigrid_data_schur_complement_preconditioner.coarse_solver]);
    }

  }

  void print_parameters_turbulence(ConditionalOStream &pcout)
  {
    pcout << std::endl
          << "Turbulence:" << std::endl;

  }

  void print_parameters_output_and_postprocessing(ConditionalOStream &pcout)
  {
    pcout << std::endl
          << "Output and postprocessing:" << std::endl;
   
    // output for visualization of results
    output_data.print(pcout, problem_type == ProblemType::Unsteady);

    // calculation of error
    error_data.print(pcout, problem_type == ProblemType::Unsteady);
     
    // output of solver information
    if(problem_type == ProblemType::Unsteady)
    {
      print_parameter(pcout,
                      "Output solver info every timesteps",
                      output_solver_info_every_timesteps);
    }
    
    // restart
    if(problem_type == ProblemType::Unsteady)
    {
      print_parameter(pcout,"Write restart",write_restart);
      if(write_restart == true)
      {
        print_parameter(pcout,"Restart interval time",restart_interval_time);
        print_parameter(pcout,"Restart interval wall time",restart_interval_wall_time);
        print_parameter(pcout,"Restart every timesteps",restart_every_timesteps);
      }
    } 
  }

  /**************************************************************************************/
  /*                                                                                    */
  /*                                 MATHEMATICAL MODEL                                 */
  /*                                                                                    */
  /**************************************************************************************/

  // description: see enum declaration
  ProblemType problem_type;

  // description: see enum declaration
  EquationType equation_type;

  // description: see enum declaration
  FormulationViscousTerm formulation_viscous_term;

  // if the body force vector on the right-hand side of the momentum equation of the
  // Navier-Stokes equations is unequal zero, set right_hand_side = true
  bool right_hand_side;



  /**************************************************************************************/
  /*                                                                                    */
  /*                                 PHYSICAL QUANTITIES                                */
  /*                                                                                    */
  /**************************************************************************************/

  // start time of simulation
  double start_time;

  // end time of simulation
  double end_time;

  // kinematic viscosity
  double viscosity;



  /**************************************************************************************/
  /*                                                                                    */
  /*                             TEMPORAL DISCRETIZATION                                */
  /*                                                                                    */
  /**************************************************************************************/

  // description: see enum declaration
  TemporalDiscretization temporal_discretization;

  // description: see enum declaration
  TreatmentOfConvectiveTerm treatment_of_convective_term;

  // description: see enum declaration
  TimeStepCalculation  calculation_of_time_step_size;

  // maximum velocity needed when calculating the time step according to cfl-condition
  double max_velocity;

  // cfl number: note that this cfl number is the first in a series of cfl numbers
  // when performing temporal convergence tests, i.e., cfl_real = cfl, cfl/2, cfl/4, ...
  double cfl;

  // C_eff: constant that has to be specified for time step calculation method
  // MaxEfficiency, which means that the time step is selected such that the errors of
  // the temporal and spatial discretization are comparable
  double c_eff;

  // user specified time step size:  note that this time_step_size is the first
  // in a series of time_step_size's when performing temporal convergence tests,
  // i.e., delta_t = time_step_size, time_step_size/2, ...
  double time_step_size;

  // maximum number of time steps
  unsigned int max_number_of_time_steps;

  // order of BDF time integration scheme and extrapolation scheme
  unsigned int order_time_integrator;

  // start time integrator with low order time integrator, i.e., first order Euler method
  bool start_with_low_order;



  /**************************************************************************************/
  /*                                                                                    */
  /*                              SPATIAL DISCRETIZATION                                */
  /*                                                                                    */
  /**************************************************************************************/

  // description: see enum declaration
  SpatialDiscretization spatial_discretization;

  // description: see enum declaration
  InteriorPenaltyFormulation IP_formulation_viscous;

  // interior penalty parameter scaling factor for Helmholtz equation of viscous step
  double IP_factor_viscous;

  // integration by parts of grad(P)
  bool gradp_integrated_by_parts;

  // use boundary data if integrated by parts
  bool gradp_use_boundary_data;

  // integration by parts of div(U)
  bool divu_integrated_by_parts;

  // use boundary data if integrated by parts
  bool divu_use_boundary_data;

  // special case of pure Dirichlet BCs on whole boundary
  bool pure_dirichlet_bc;



  /**************************************************************************************/
  /*                                                                                    */
  /*                        HIGH-ORDER DUAL SPLITTING SCHEME                            */
  /*                                                                                    */
  /**************************************************************************************/

  // interior penalty parameter scaling factor for pressure Poisson equation
  double IP_factor_pressure;

  // description: see enum declaration
  PreconditionerPressurePoisson preconditioner_pressure_poisson;

  // description: see declaration of MultigridData
  MultigridData multigrid_data_pressure_poisson;

  // solver tolerances for pressure Poisson equation
  double abs_tol_pressure;
  double rel_tol_pressure;

  // use small time steps stability approach (similar to approach of Leriche et al.)
  bool small_time_steps_stability;

  // use approach of Ferrer et al. (increase penalty parameter when reducing
  // the time step in order to improve stability in the limit of small time steps)
  bool use_approach_of_ferrer;

  // reference time step size that is used when use_approach_of_ferrer == true
  double deltat_ref;

  // description: see enum declaration
  ProjectionType projection_type;

  // penalty factor of divergence penalty term in projection step
  double penalty_factor_divergence;

  // penalty factor of divergence penalty term in projection step
  double penalty_factor_continuity;

  // description: see enum declaration
  SolverProjection solver_projection;

  // description: see enum declaration
  PreconditionerProjection preconditioner_projection;

  // solver tolerances for projection step
  double abs_tol_projection;
  double rel_tol_projection;

  // description: see enum declaration
  SolverViscous solver_viscous;

  // description: see enum declaration
  PreconditionerViscous preconditioner_viscous;

  // description: see declaration of MultigridData
  MultigridData multigrid_data_viscous;

  // solver tolerances for Helmholtz equation of viscous step
  double abs_tol_viscous;
  double rel_tol_viscous;



  /**************************************************************************************/
  /*                                                                                    */
  /*                            COUPLED NAVIER-STOKES SOLVER                            */
  /*                                                                                    */
  /**************************************************************************************/

  // use symmetric saddle point matrix for coupled solver:
  // continuity equation formulated as: - div(u) = 0 -> symmetric formulation
  //                                      div(u) = 0 -> non-symmetric formulation
//  bool use_symmetric_saddle_point_matrix;

  // solver tolerances Newton solver
  double abs_tol_newton;
  double rel_tol_newton;
  unsigned int max_iter_newton;

  // description: see enum declaration
  SolverLinearizedNavierStokes solver_linearized_navier_stokes;

  // solver tolerances for linearized problem of Newton solver
  double abs_tol_linear;
  double rel_tol_linear;
  unsigned int max_iter_linear;

  // description: see enum declaration
  PreconditionerLinearizedNavierStokes preconditioner_linearized_navier_stokes;

  // use right preconditioning
  bool use_right_preconditioning;

  // defines the maximum size of the Krylov subspace before restart
  unsigned int max_n_tmp_vectors;

  // description: see enum declaration
  MomentumPreconditioner momentum_preconditioner;

  // description: see enum declaration
  SolverMomentumPreconditioner solver_momentum_preconditioner;

  // description: see declaration
  MultigridData multigrid_data_momentum_preconditioner;

  // relative tolerance for solver_momentum_preconditioner
  double rel_tol_solver_momentum_preconditioner;

  // description: see enum declaration
  SchurComplementPreconditioner schur_complement_preconditioner;

  // description: see enum declaration
  DiscretizationOfLaplacian discretization_of_laplacian;

  // description: see enum declaration
  SolverSchurComplementPreconditioner solver_schur_complement_preconditioner;

  // description: see declaration
  MultigridData multigrid_data_schur_complement_preconditioner;

  // relative tolerance for solver_schur_complement_preconditioner
  double rel_tol_solver_schur_complement_preconditioner;



  /**************************************************************************************/
  /*                                                                                    */
  /*                                     TURBULENCE                                     */
  /*                                                                                    */
  /**************************************************************************************/

  // before then no statistics calculation will be performed
  double statistics_start_time;

  // calculate statistics every "statistics_every" time steps
  unsigned int statistics_every;

  // Smagorinsky constant
  double cs;

  // mixing-length model for xwall
  double ml;

  // xwall with adaptive wall shear stress
  bool variabletauw;

  // delta tauw if adaptive between 0 and 1
  double dtauw;

  // max wall distance of enriched elements
  double max_wdist_xwall;


  /**************************************************************************************/
  /*                                                                                    */
  /*                               OUTPUT AND POSTPROCESSING                            */
  /*                                                                                    */
  /**************************************************************************************/

  // print input parameters at the beginning of the simulation
  bool print_input_parameters;

  // writing output for visualization
  OutputDataNavierStokes output_data;

  // calculating errors
  ErrorCalculationData error_data;

  // show solver performance (wall time, number of iterations) every ... timesteps
  unsigned int output_solver_info_every_timesteps;

  // write restart
  bool write_restart;

  // specifies the time interval in which restarts are written, starting from start_time
  double restart_interval_time;

  // specifies the wall time interwal in which restarts are written
  double restart_interval_wall_time;

  // specifies the restart interval via number of time steps
  unsigned int restart_every_timesteps;

  // computation of lift and drag coefficients
  LiftAndDragData lift_and_drag_data;

  // computation of pressure difference between two points
  PressureDifferenceData<dim> pressure_difference_data;

  // analysis of mass conservation
  MassConservationData mass_data;
};

#endif /* INCLUDE_INPUTPARAMETERSNAVIERSTOKES_H_ */