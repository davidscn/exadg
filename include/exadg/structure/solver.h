/*
 * solver.h
 *
 *  Created on: Jan 19, 2021
 *      Author: fehn
 */

#ifndef INCLUDE_EXADG_STRUCTURE_SOLVER_H_
#define INCLUDE_EXADG_STRUCTURE_SOLVER_H_

// deal.II
#include <deal.II/base/exceptions.h>
#include <deal.II/base/parameter_handler.h>

// ExaDG

// driver
#include <exadg/structure/driver.h>

// utilities
#include <exadg/utilities/convergence_study.h>

namespace ExaDG
{
// forward declarations
template<int dim, typename Number>
std::shared_ptr<Structure::ApplicationBase<dim, Number>>
get_application(std::string input_file);

template<int dim, typename Number>
void
add_parameters_application(dealii::ParameterHandler & prm, std::string const & input_file);

void
create_input_file(std::string const & input_file)
{
  dealii::ParameterHandler prm;

  ConvergenceStudy study;
  study.add_parameters(prm);

  // we have to assume a default dimension and default Number type
  // for the automatic generation of a default input file
  unsigned int const Dim = 2;
  typedef double     Number;

  add_parameters_application<Dim, Number>(prm, input_file);

  prm.print_parameters(input_file,
                       dealii::ParameterHandler::Short |
                         dealii::ParameterHandler::KeepDeclarationOrder);
}

template<int dim, typename Number>
void
run(std::string const & input_file,
    unsigned int const  degree,
    unsigned int const  refine_space,
    unsigned int const  refine_time,
    MPI_Comm const &    mpi_comm)
{
  Timer timer;
  timer.restart();

  std::shared_ptr<Structure::Driver<dim, Number>> solver;
  solver.reset(new Structure::Driver<dim, Number>(mpi_comm));

  std::shared_ptr<Structure::ApplicationBase<dim, Number>> application =
    get_application<dim, Number>(input_file);

  solver->setup(application, degree, refine_space, refine_time);

  solver->solve();

  solver->print_statistics(timer.wall_time());
}
} // namespace ExaDG

int
main(int argc, char ** argv)
{
  dealii::Utilities::MPI::MPI_InitFinalize mpi(argc, argv, 1);

  MPI_Comm mpi_comm(MPI_COMM_WORLD);

  std::string input_file;

  if(argc == 1 or (argc == 2 and std::string(argv[1]) == "--help"))
  {
    if(dealii::Utilities::MPI::this_mpi_process(mpi_comm) == 0)
    {
      std::cout << "To run the program, use:      ./solver input_file" << std::endl
                << "To create an input file, use: ./solver --create_input_file input_file"
                << std::endl;
    }

    return 0;
  }
  else if(argc >= 2)
  {
    input_file = std::string(argv[argc - 1]);
  }

  if(argc == 3 and std::string(argv[1]) == "--create_input_file")
  {
    if(dealii::Utilities::MPI::this_mpi_process(mpi_comm) == 0)
      ExaDG::create_input_file(input_file);

    return 0;
  }

  ExaDG::ConvergenceStudy study(input_file);

  // k-refinement
  for(unsigned int degree = study.degree_min; degree <= study.degree_max; ++degree)
  {
    // h-refinement
    for(unsigned int refine_space = study.refine_space_min; refine_space <= study.refine_space_max;
        ++refine_space)
    {
      // dt-refinement
      for(unsigned int refine_time = study.refine_time_min; refine_time <= study.refine_time_max;
          ++refine_time)
      {
        // run the simulation
        if(study.dim == 2 && study.precision == "float")
          ExaDG::run<2, float>(input_file, degree, refine_space, refine_time, mpi_comm);
        else if(study.dim == 2 && study.precision == "double")
          ExaDG::run<2, double>(input_file, degree, refine_space, refine_time, mpi_comm);
        else if(study.dim == 3 && study.precision == "float")
          ExaDG::run<3, float>(input_file, degree, refine_space, refine_time, mpi_comm);
        else if(study.dim == 3 && study.precision == "double")
          ExaDG::run<3, double>(input_file, degree, refine_space, refine_time, mpi_comm);
        else
          AssertThrow(false,
                      dealii::ExcMessage("Only dim = 2|3 and precision=float|double implemented."));
      }
    }
  }

  return 0;
}

#endif /* INCLUDE_EXADG_STRUCTURE_SOLVER_H_ */