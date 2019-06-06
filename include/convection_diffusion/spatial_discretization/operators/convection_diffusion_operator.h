#ifndef CONV_DIFF_CONVECTION_DIFFUSION_OPERATOR
#define CONV_DIFF_CONVECTION_DIFFUSION_OPERATOR

#include "../../../operators/operator_base.h"
#include "../../user_interface/boundary_descriptor.h"
#include "../../user_interface/input_parameters.h"

#include "convective_operator.h"
#include "diffusive_operator.h"
#include "mass_operator.h"

#include "../../../operators/elementwise_operator.h"
#include "../../../solvers_and_preconditioners/preconditioner/elementwise_preconditioners.h"
#include "../../../solvers_and_preconditioners/solvers/wrapper_elementwise_solvers.h"

namespace ConvDiff
{
template<int dim>
struct ConvectionDiffusionOperatorData : public OperatorBaseData
{
  ConvectionDiffusionOperatorData()
    : OperatorBaseData(0, 0),
      unsteady_problem(true),
      convective_problem(true),
      diffusive_problem(true),
      dof_index_velocity(1),
      type_velocity_field(TypeVelocityField::Analytical),
      scaling_factor_time_derivative_term(-1.0),
      preconditioner_block_jacobi(PreconditionerBlockDiagonal::InverseMassMatrix),
      block_jacobi_solver_data(SolverData(1000, 1.e-12, 1.e-2, 1000)),
      mg_operator_type(MultigridOperatorType::Undefined)
  {
  }

  void
  update_mapping_update_flags()
  {
    if(unsteady_problem)
      this->append_mapping_update_flags(mass_matrix_operator_data);
    if(convective_problem)
      this->append_mapping_update_flags(convective_operator_data);
    if(diffusive_problem)
      this->append_mapping_update_flags(diffusive_operator_data);
  }

  bool unsteady_problem;
  bool convective_problem;
  bool diffusive_problem;

  // only relevant for TypeVelocityField::Numerical
  unsigned int      dof_index_velocity;
  TypeVelocityField type_velocity_field;

  double scaling_factor_time_derivative_term;

  MassMatrixOperatorData      mass_matrix_operator_data;
  ConvectiveOperatorData<dim> convective_operator_data;
  DiffusiveOperatorData<dim>  diffusive_operator_data;

  // elementwise iterative solution of block Jacobi problems
  PreconditionerBlockDiagonal preconditioner_block_jacobi;
  SolverData                  block_jacobi_solver_data;

  MultigridOperatorType mg_operator_type;
};

template<int dim, typename Number = double>
class ConvectionDiffusionOperator
  : public OperatorBase<dim, Number, ConvectionDiffusionOperatorData<dim>>
{
public:
  typedef Number value_type;

private:
  typedef OperatorBase<dim, value_type, ConvectionDiffusionOperatorData<dim>> Base;
  typedef ConvectionDiffusionOperator<dim, Number>                            This;

  typedef typename Base::BlockMatrix BlockMatrix;

  typedef typename Base::VectorType VectorType;

#ifdef DEAL_II_WITH_TRILINOS
  typedef typename Base::SparseMatrix SparseMatrix;
#endif

public:
  ConvectionDiffusionOperator();

  void
  reinit(MatrixFree<dim, Number> const &              mf_data,
         AffineConstraints<double> const &            constraint_matrix,
         ConvectionDiffusionOperatorData<dim> const & operator_data) const;

  void
  reinit(MatrixFree<dim, Number> const &              mf_data,
         AffineConstraints<double> const &            constraint_matrix,
         ConvectionDiffusionOperatorData<dim> const & operator_data,
         MassMatrixOperator<dim, Number> const &      mass_matrix_operator,
         ConvectiveOperator<dim, Number> const &      convective_operator,
         DiffusiveOperator<dim, Number> const &       diffusive_operator) const;

  /*
   *  Scaling factor of time derivative term (mass matrix term)
   */
  void
  set_scaling_factor_time_derivative_term(double const & factor);

  double
  get_scaling_factor_time_derivative_term() const;

  std::shared_ptr<BoundaryDescriptor<dim>>
  get_boundary_descriptor() const;

  LinearAlgebra::distributed::Vector<Number> const &
  get_velocity() const;

  void
  set_velocity(VectorType const & velocity) const;

  // overwrite function of base class since this is a combined operator
  void
  apply(VectorType & dst, VectorType const & src) const;

  // overwrite function of base class since this is a combined operator
  void
  apply_add(VectorType & dst, VectorType const & src) const;

#ifdef DEAL_II_WITH_TRILINOS
  void
  calculate_system_matrix(SparseMatrix & system_matrix) const;
#endif

  /*
   * Diagonal preconditioner.
   */

  void
  calculate_diagonal(VectorType & diagonal) const;

  /*
   * Block diagonal preconditioner.
   */

  // apply the inverse block diagonal operator (for matrix-based and matrix-free variants)
  void
  apply_inverse_block_diagonal(VectorType & dst, VectorType const & src) const;

  void
  apply_add_block_diagonal_elementwise(unsigned int const                    cell,
                                       VectorizedArray<Number> * const       dst,
                                       VectorizedArray<Number> const * const src,
                                       unsigned int const problem_size = 1) const;

private:
  /*
   * This function calculates the block Jacobi matrices and adds the result to matrices. This is
   * done sequentially for the different operators.
   */
  void
  add_block_diagonal_matrices(BlockMatrix & matrices) const;

  void
  initialize_block_diagonal_preconditioner_matrix_free() const;

  mutable lazy_ptr<MassMatrixOperator<dim, Number>> mass_matrix_operator;
  mutable lazy_ptr<ConvectiveOperator<dim, Number>> convective_operator;
  mutable lazy_ptr<DiffusiveOperator<dim, Number>>  diffusive_operator;

  mutable VectorType temp;

  mutable double scaling_factor_time_derivative_term;

  // Block Jacobi preconditioner/smoother: matrix-free version with elementwise iterative solver
  typedef Elementwise::OperatorBase<dim, Number, This> ELEMENTWISE_OPERATOR;

  typedef Elementwise::PreconditionerBase<VectorizedArray<Number>> PRECONDITIONER_BASE;

  typedef Elementwise::
    IterativeSolver<dim, 1 /*scalar equation*/, Number, ELEMENTWISE_OPERATOR, PRECONDITIONER_BASE>
      ELEMENTWISE_SOLVER;

  mutable std::shared_ptr<ELEMENTWISE_OPERATOR> elementwise_operator;
  mutable std::shared_ptr<PRECONDITIONER_BASE>  elementwise_preconditioner;
  mutable std::shared_ptr<ELEMENTWISE_SOLVER>   elementwise_solver;
};

} // namespace ConvDiff

#endif
