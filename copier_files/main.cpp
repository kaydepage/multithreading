/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include "writer.h"
#include <time.h>
#include <cstdlib>
/* global variables if needed go here */
int main(int argc, char** argv) {
   if (!(argc >= 3)) {
      std::cerr << argc << "Incorrect number of arguements: Missing input or output files." << std::endl;
      return EXIT_FAILURE;
   }
   bool useTiming{false};
   if (argc == 4 && std::string(argv[3]) == "-t") {
      useTiming = true;
      reader::useTiming = true;
   } 
   clock_t start;
   clock_t end;
   std::string inputFile = argv[1];
   std::string outputFile = argv[2];
    /* check command line arguments */
    /* load the file and copy it to the destination */
   writer Writer(outputFile);
   reader Reader(inputFile , Writer);
   std::cout << "Beginning Reading" << std::endl;
   if (useTiming) {
      start = clock();
   }
   
   Reader.run();
   std::cout << "Writing Finished" << std::endl;

   if (useTiming) {
      end = clock();
      std::cout << "Execution Time: " << static_cast<double>(end - start) / (double)CLOCKS_PER_SEC << std::endl;

   }
   
   return EXIT_SUCCESS;
}
 