#ifndef HEMELB_FILEMANAGER_H
#define HEMELB_FILEMANAGER_H

#include <string>
#include "configuration/CommandLine.h"
#include "util/fileutils.h"
#include "log/Logger.h"
#include "configuration/SimConfig.h"
#include "io/XdrFileWriter.h"
namespace hemelb
{
  namespace reporting
  {
    class FileManager
    {
      public:
        FileManager(configuration::CommandLine & commandLine, const bool &io, const int &processorCount);
        ~FileManager();
        bool HasProblems() const
        {
          return(!ok);
        }
        void ReportPhase1(long int site_count, int total_time_steps, long int cycle_id, double simulation_time, bool unstable,
                          unsigned long time_steps_per_cycle,
                          double decomposition_time, double initialise_time, double read_time, double creation_time);
        void ReportProcessorTimings(std::string *const names,double *const mins,double *const means,double *const maxes);
        const std::string & GetInputFile() const;
        const std::string & GetSnapshotDirectory() const;
        const std::string & GetImageDirectory() const;
        FILE * ReportFile() {return(mTimingsFile);}
        void SaveConfiguration(configuration::SimConfig * simConfig);
        void EmptyOutputDirectories();
        hemelb::io::XdrFileWriter * XdrImageWriter(const long int time);
        const std::string SnapshotPath(unsigned long time) const;
      private:
        void ReportHeader();
        void GuessOutputDir();
        void InitialiseReport(const int & processorCount);
        FILE *mTimingsFile;
        std::string outputDir;
        std::string inputFile;
        std::string snapshotDirectory;
        std::string imageDirectory;
        std::string configLeafName;
        configuration::CommandLine &options;
        bool ok;
        bool doIo;
    };
  }
}

#endif