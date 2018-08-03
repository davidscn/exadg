/*
 * DGLaplaceOperation.h
 *
 *  Created on:
 *      Author:
 */

#ifndef INCLUDE_CONVECTION_DIFFUSION_DG_CONVECTION_DIFFUSION_OPERATION_H_
#define INCLUDE_CONVECTION_DIFFUSION_DG_CONVECTION_DIFFUSION_OPERATION_H_

#include <deal.II/fe/fe_dgq.h>
#include <deal.II/fe/fe_values.h>
#include <deal.II/fe/mapping_q.h>
#include <deal.II/lac/parallel_vector.h>
#include <deal.II/numerics/vector_tools.h>

#include "../../operators/inverse_mass_matrix.h"
#include "../../operators/matrix_operator_base.h"
#include "../../solvers_and_preconditioners/preconditioner/inverse_mass_matrix_preconditioner.h"
#include "../../solvers_and_preconditioners/preconditioner/jacobi_preconditioner.h"
#include "../../solvers_and_preconditioners/solvers/iterative_solvers.h"

#include "../preconditioners/multigrid_preconditioner.h"
#include "../user_interface/boundary_descriptor.h"
#include "../user_interface/field_functions.h"
#include "../user_interface/input_parameters.h"

#include "laplace_operator.h"
#include "rhs_operator.h"

namespace Laplace {

template <int dim, int fe_degree, typename value_type>
class DGOperation : public MatrixOperatorBase {
public:
  typedef parallel::distributed::Vector<value_type> VNumber;

  DGOperation(parallel::distributed::Triangulation<dim> const &triangulation,
              Laplace::InputParameters const &param_in);

  void setup(
      const std::vector<GridTools::PeriodicFacePair<
          typename Triangulation<dim>::cell_iterator>>
          periodic_face_pairs,
      std::shared_ptr<Laplace::BoundaryDescriptor<dim>> boundary_descriptor_in,
      std::shared_ptr<Laplace::FieldFunctions<dim>> field_functions_in);

  void setup_solver();

  void initialize_dof_vector(VNumber &src) const;

  void rhs(VNumber &dst, double const evaluation_time = 0.0) const;

  unsigned int solve(VNumber &sol, VNumber const &rhs);

  MatrixFree<dim, value_type> const &get_data() const;

  Mapping<dim> const &get_mapping() const;

  DoFHandler<dim> const &get_dof_handler() const;

private:
  void create_dofs();

  void initialize_matrix_free();

  void setup_operators();

  FE_DGQ<dim> fe;
  MappingQGeneric<dim> mapping;
  DoFHandler<dim> dof_handler;

  MatrixFree<dim, value_type> data;

  Laplace::InputParameters const &param;

  std::vector<
      GridTools::PeriodicFacePair<typename Triangulation<dim>::cell_iterator>>
      periodic_face_pairs;

  std::shared_ptr<Laplace::BoundaryDescriptor<dim>> boundary_descriptor;
  std::shared_ptr<Laplace::FieldFunctions<dim>> field_functions;

  Laplace::RHSOperator<dim, fe_degree, value_type> rhs_operator;

  Laplace::LaplaceOperator<dim, fe_degree, value_type> laplace_operator;

  std::shared_ptr<PreconditionerBase<value_type>> preconditioner;
  std::shared_ptr<IterativeSolverBase<VNumber>> iterative_solver;
};
}

#include "poisson_operation.cpp"

#endif
