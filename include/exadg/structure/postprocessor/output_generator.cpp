/*  ______________________________________________________________________
 *
 *  ExaDG - High-Order Discontinuous Galerkin for the Exa-Scale
 *
 *  Copyright (C) 2021 by the ExaDG authors
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

// deal.II
#include <deal.II/grid/grid_out.h>
#include <deal.II/numerics/data_out.h>
#include <deal.II/numerics/data_out_dof_data.h>

// ExaDG
#include <exadg/postprocessor/write_output.h>
#include <exadg/structure/postprocessor/output_generator.h>
#include <exadg/utilities/create_directories.h>

namespace ExaDG
{
namespace Structure
{
template<int dim, typename VectorType>
void
write_output(OutputDataBase const &          output_data,
             dealii::DoFHandler<dim> const & dof_handler,
             dealii::Mapping<dim> const &    mapping,
             VectorType const &              solution_vector,
             unsigned int const              output_counter,
             MPI_Comm const &                mpi_comm)
{
  dealii::DataOutBase::VtkFlags flags;
  flags.write_higher_order_cells = output_data.write_higher_order;

  dealii::DataOut<dim> data_out;
  data_out.set_flags(flags);

  std::vector<std::string> names(dim, "displacement");
  std::vector<dealii::DataComponentInterpretation::DataComponentInterpretation>
    component_interpretation(dim, dealii::DataComponentInterpretation::component_is_part_of_vector);

#if !DEAL_II_VERSION_GTE(9, 4, 0)
  solution_vector.update_ghost_values();
#endif

  data_out.add_data_vector(dof_handler, solution_vector, names, component_interpretation);

  data_out.build_patches(mapping, output_data.degree, dealii::DataOut<dim>::curved_inner_cells);

  data_out.write_vtu_with_pvtu_record(
    output_data.directory, output_data.filename, output_counter, mpi_comm, 4);
}

template<int dim, typename Number>
OutputGenerator<dim, Number>::OutputGenerator(MPI_Comm const & comm)
  : mpi_comm(comm), output_counter(0), reset_counter(true)
{
}

template<int dim, typename Number>
void
OutputGenerator<dim, Number>::setup(dealii::DoFHandler<dim> const & dof_handler_in,
                                    dealii::Mapping<dim> const &    mapping_in,
                                    OutputDataBase const &          output_data_in)
{
  dof_handler = &dof_handler_in;
  mapping     = &mapping_in;
  output_data = output_data_in;

  // reset output counter
  output_counter = output_data.start_counter;

  if(output_data.write_output == true)
  {
    create_directories(output_data.directory, mpi_comm);

    // Visualize boundary IDs:
    // since boundary IDs typically do not change during the simulation, we only do this
    // once at the beginning of the simulation (i.e., in the setup function).
    if(output_data.write_boundary_IDs)
    {
      write_boundary_IDs(dof_handler->get_triangulation(),
                         output_data.directory,
                         output_data.filename,
                         mpi_comm);
    }

    // write surface mesh
    if(output_data.write_surface_mesh)
    {
      write_surface_mesh(dof_handler->get_triangulation(),
                         *mapping,
                         output_data.degree,
                         output_data.directory,
                         output_data.filename,
                         0,
                         mpi_comm);
    }

    // processor_id
    if(output_data.write_processor_id)
    {
      dealii::GridOut grid_out;

      grid_out.write_mesh_per_processor_as_vtu(dof_handler->get_triangulation(),
                                               output_data.directory + output_data.filename +
                                                 "_processor_id");
    }
  }
}

template<int dim, typename Number>
void
OutputGenerator<dim, Number>::evaluate(VectorType const & solution,
                                       double const &     time,
                                       int const &        time_step_number)
{
  dealii::ConditionalOStream pcout(std::cout,
                                   dealii::Utilities::MPI::this_mpi_process(MPI_COMM_WORLD) == 0);

  if(output_data.write_output == true)
  {
    if(time_step_number >= 0) // unsteady problem
    {
      // small number which is much smaller than the time step size
      double const EPSILON = 1.0e-10;

      // The current time might be larger than start_time. In that case, we first have to
      // reset the counter in order to avoid that output is written every time step.
      if(reset_counter)
      {
        output_counter +=
          int((time - output_data.start_time + EPSILON) / output_data.interval_time);
        reset_counter = false;
      }

      if(time > (output_data.start_time + output_counter * output_data.interval_time - EPSILON))
      {
        pcout << std::endl
              << "OUTPUT << Write data at time t = " << std::scientific << std::setprecision(4)
              << time << std::endl;

        write_output<dim>(output_data, *dof_handler, *mapping, solution, output_counter, mpi_comm);

        ++output_counter;
      }
    }
    else // steady problem (time_step_number = -1)
    {
      pcout << std::endl
            << "OUTPUT << Write " << (output_counter == 0 ? "initial" : "solution") << " data"
            << std::endl;

      write_output<dim>(output_data, *dof_handler, *mapping, solution, output_counter, mpi_comm);

      ++output_counter;
    }
  }
}

template class OutputGenerator<2, float>;
template class OutputGenerator<3, float>;

template class OutputGenerator<2, double>;
template class OutputGenerator<3, double>;

} // namespace Structure
} // namespace ExaDG
