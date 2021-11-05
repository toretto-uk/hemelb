// This file is part of HemeLB and is Copyright (C)
// the HemeLB team and/or their institutions, as detailed in the
// file AUTHORS. This software is provided under the terms of the
// license in the file LICENSE.

#include <memory>
#include <boost/uuid/uuid_io.hpp>
#include <catch2/catch.hpp>

#include "SimulationMaster.h"
#include "lb/BuildSystemInterface.h"
#include "lb/lattices/D3Q19.h"
#include "Traits.h"
#include "redblood/Mesh.h"
#include "redblood/Cell.h"
#include "redblood/CellController.h"
#include "redblood/stencil.h"
#include "tests/helpers/FolderTestFixture.h"

namespace hemelb
{
  namespace tests
  {
    using namespace redblood;

    TEST_CASE_METHOD(helpers::FolderTestFixture, "Fedosov validation tests", "[redblood][.long]") {

      typedef Traits<>::Reinstantiate<lb::lattices::D3Q19, lb::GuoForcingLBGK>::Type Traits;
      typedef hemelb::redblood::CellController<Traits> CellControl;
      typedef SimulationMaster<Traits> MasterSim;

      CopyResourceToTempdir("fedosov1c.xml");
      CopyResourceToTempdir("fedosov1c.gmy");
      CopyResourceToTempdir("rbc_ico_720.msh");
      CopyResourceToTempdir("rbc_ico_1280.msh");
      int const argc = 7;
      char const * argv[7];
      argv[0] = "hemelb";
      argv[1] = "-in";
      argv[2] = "fedosov1c.xml";
      argv[3] = "-i";
      argv[4] = "0";
      argv[5] = "-ss";
      argv[6] = "1111";

      configuration::CommandLine options(argc, argv);
      auto master = std::make_shared<MasterSim>(options, Comms());
      redblood::VTKMeshIO vtk_io = {};

      SECTION("Integration test") {
	REQUIRE(master);
	auto const & converter = master->GetUnitConverter();
	auto controller = std::static_pointer_cast<CellControl>(master->GetCellController());
	REQUIRE(controller);
	controller->AddCellChangeListener([vtk_io, &converter](const hemelb::redblood::CellContainer &cells) {
	    static int iter = 0;
	    if(cells.empty()) {
	      return;
	    }
	    auto cell = *cells.begin();
	    if(iter % 1000 == 0) {
	      std::stringstream filename;
	      filename << cell->GetTag() << "_t_" << iter << ".vtp";
	      vtk_io.writeFile(filename.str(), *cell, converter);
	    }
	    ++iter;
	  });

	// run the simulation
	master->RunSimulation();
	master->Finalise();

	AssertPresent("results/report.txt");
	AssertPresent("results/report.xml");
      }
    }

  } // namespace tests
} // namespace hemelb
