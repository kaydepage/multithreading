/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include "writer.h"
#include <cstdlib>

int main(int argc, char** argv) {
   bool useTiming{false};
    /**
     * check command line arguments
     **/
    /**
     * process command line arguments
     **/
    /**
     * initiliaze the reader and writer classes
     **/
    /**
     * initialize the running of each thread. Note you just call run() on each
     * object here, you'll call pthread_create itself in the run function.
     **/
   if (!(argc >= 4)) {
      std::cerr << "Usage: " << argv[0] << "<num_threads> <input_file> <output_file>" << std::endl;
      return EXIT_FAILURE;        
    }

   int numThreads = std::stoi(argv[1]);
   
   reader reader(numThreads);
   writer writer(numThreads);

   if (argc == 5 && std::string(argv[4]) == "-t") {
      useTiming = true;
      reader::useTiming = true;
      writer::useTiming = true; 
   } 

   writer.init(argv[3]);
   reader.init(argv[2]);
   
   std::cout << "starting" << std::endl;
   reader.run();
   writer.run();

   clock_t start;
   clock_t end;
   if (useTiming) {
      start = clock();
   }
   for (int i = 0; i < numThreads; ++i) {
      pthread_join(writer.writerThreadArray[i], NULL);
      pthread_join(reader.readerThreadArray[i], NULL);
   }
   if (useTiming) {
      end = clock();
      std::cout << "---------------" << std::endl;
      std::cout << "Total Execution Time: " << static_cast<double>(end - start) / (double)CLOCKS_PER_SEC << std::endl;
      std::cout << "---------------" << std::endl;
   }

   //std::cout << "RR: " << totalReaderTime / (double)CLOCKS_PER_SEC << std::endl;
    /**
     *
     * have loop here waiting for the threads to bomplete. Please see
     * section on avoiding busy waiting on the assignment specification to see
     * what need to be done here
      **/
   
   return EXIT_SUCCESS;
}
