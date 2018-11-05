#ifndef LAPLACE_OPERATOR_H
#define LAPLACE_OPERATOR_H

#include "../../operators/interior_penalty_parameter.h"
#include "../../operators/operator_base.h"
#include "../../operators/operator_type.h"

#include "../user_interface/boundary_descriptor.h"

namespace Poisson
{
template<int dim>
struct LaplaceOperatorData : public OperatorBaseData<dim>
{
public:
  LaplaceOperatorData()
    // clang-format off
    : OperatorBaseData<dim>(0, 0,
          false, true, false, false, true, false, // cell
          true,  true,        true,  true         // face
      ),
      // clang-format on
      IP_factor(1.0)
  {
    this->mapping_update_flags = update_gradients | update_JxW_values;
    this->mapping_update_flags_inner_faces =
      this->mapping_update_flags | update_values | update_normal_vectors;
    this->mapping_update_flags_boundary_faces =
      this->mapping_update_flags_inner_faces | update_quadrature_points;
  }

  double IP_factor;

  std::shared_ptr<Poisson::BoundaryDescriptor<dim>> bc;
};

template<int dim, int degree, typename Number>
class LaplaceOperator : public OperatorBase<dim, degree, Number, LaplaceOperatorData<dim>>
{
public:
  typedef LaplaceOperator<dim, degree, Number>                        This;
  typedef OperatorBase<dim, degree, Number, LaplaceOperatorData<dim>> Parent;
  typedef typename Parent::FEEvalCell                                 FEEvalCell;
  typedef typename Parent::FEEvalFace                                 FEEvalFace;

  typedef typename Parent::VectorType VectorType;

  typedef VectorizedArray<Number> scalar;

  // static constants
  static const int DIM = Parent::DIM;

  LaplaceOperator();

  void
  initialize(Mapping<dim> const &             mapping,
             MatrixFree<dim, Number> const &  mf_data,
             LaplaceOperatorData<dim> const & operator_data)
  {
    ConstraintMatrix constraint_matrix;
    Parent::reinit(mf_data, constraint_matrix, operator_data);

    // calculate penalty parameters
    IP::calculate_penalty_parameter<dim, degree, Number>(array_penalty_parameter,
                                                         *this->data,
                                                         mapping,
                                                         this->operator_data.dof_index);
  }

  void
  initialize(Mapping<dim> const &             mapping,
             MatrixFree<dim, Number> &        mf_data,
             ConstraintMatrix &               constraint_matrix,
             LaplaceOperatorData<dim> const & operator_data)
  {
    Parent::reinit(mf_data, constraint_matrix, operator_data);

    // calculate penalty parameters
    IP::calculate_penalty_parameter<dim, degree, Number>(array_penalty_parameter,
                                                         *this->data,
                                                         mapping,
                                                         this->operator_data.dof_index);
  }

  void
  reinit(DoFHandler<dim> const &   dof_handler,
         Mapping<dim> const &      mapping,
         void *                    operator_data,
         MGConstrainedDoFs const & mg_constrained_dofs,
         unsigned int const        level)
  {
    Parent::reinit(dof_handler, mapping, operator_data, mg_constrained_dofs, level);

    // calculate penalty parameters
    IP::calculate_penalty_parameter<dim, degree, Number>(array_penalty_parameter,
                                                         *this->data,
                                                         mapping,
                                                         this->operator_data.dof_index);
  }

private:
  inline DEAL_II_ALWAYS_INLINE //
    scalar
    calculate_value_flux(scalar const & jump_value) const;

  inline DEAL_II_ALWAYS_INLINE //
    scalar
    calculate_interior_value(unsigned int const   q,
                             FEEvalFace const &   fe_eval,
                             OperatorType const & operator_type) const;

  inline DEAL_II_ALWAYS_INLINE //
    scalar
    calculate_exterior_value(scalar const &           value_m,
                             unsigned int const       q,
                             FEEvalFace const &       fe_eval,
                             OperatorType const &     operator_type,
                             BoundaryType const &     boundary_type,
                             types::boundary_id const boundary_id) const;

  inline DEAL_II_ALWAYS_INLINE //
    scalar
    calculate_gradient_flux(scalar const & normal_gradient_m,
                            scalar const & normal_gradient_p,
                            scalar const & jump_value,
                            scalar const & penalty_parameter) const;

  inline DEAL_II_ALWAYS_INLINE //
    scalar
    calculate_interior_normal_gradient(unsigned int const   q,
                                       FEEvalFace const &   fe_eval,
                                       OperatorType const & operator_type) const;

  inline DEAL_II_ALWAYS_INLINE //
    scalar
    calculate_exterior_normal_gradient(scalar const &           normal_gradient_m,
                                       unsigned int const       q,
                                       FEEvalFace const &       fe_eval,
                                       OperatorType const &     operator_type,
                                       BoundaryType const &     boundary_type,
                                       types::boundary_id const boundary_id) const;

  void
  do_cell_integral(FEEvalCell & fe_eval) const;

  void
  do_face_integral(FEEvalFace & fe_eval, FEEvalFace & fe_eval_neighbor) const;

  void
  do_face_int_integral(FEEvalFace & fe_eval, FEEvalFace & fe_eval_neighbor) const;

  void
  do_face_ext_integral(FEEvalFace & fe_eval, FEEvalFace & fe_eval_neighbor) const;

  void
  do_boundary_integral(FEEvalFace &               fe_eval,
                       OperatorType const &       operator_type,
                       types::boundary_id const & boundary_id) const;

  MultigridOperatorBase<dim, Number> *
  get_new(unsigned int deg) const;

  void
  do_verify_boundary_conditions(types::boundary_id const             boundary_id,
                                LaplaceOperatorData<dim> const &     operator_data,
                                std::set<types::boundary_id> const & periodic_boundary_ids) const;

  // stores the penalty parameter of the interior penalty method for each cell
  AlignedVector<scalar> array_penalty_parameter;
};

} // namespace Poisson

#endif