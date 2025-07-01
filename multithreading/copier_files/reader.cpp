/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include <functional>
bool reader::useTiming{false};

reader::reader(const std::string& name, writer& mywriter) : in(name.c_str()), thewriter(mywriter) {
   std::cout << "Extracting Lines from " << name << std::endl;
   readerTime = 0;
   appendTime = 0;
   writerTime = 0;
}


void reader::run() {
   clock_t readerStart;
   clock_t readerEnd;
   clock_t appendStart;
   clock_t appendEnd;
   clock_t writerStart;
   clock_t writerEnd;
   while (in) {
      //std::cout << "Copying line " << counter;
      std::string input{};
      if (useTiming) {
         readerStart = clock();
      }
       
      std::getline(in, input);
      if (useTiming) {
         readerEnd = clock();
         appendStart = clock();
      }
      //std::cout << " Copied: " << input << std::endl;
      thewriter.append(input);
      if (useTiming) {
         appendEnd = clock();
         writerStart = clock();
      }

      thewriter.run();
      if (useTiming) {
         writerEnd = clock();
         readerTime += static_cast<double>(readerEnd - readerStart);
         appendTime += static_cast<double>(appendEnd - appendStart);
         writerTime += static_cast<double>(writerEnd - writerStart);
      }
      
   }
   if (useTiming) {   
      std::cout << "reader time: " << readerTime / (double)CLOCKS_PER_SEC << std::endl;
      std::cout << "append time: " << appendTime / (double)CLOCKS_PER_SEC << std::endl;
      std::cout << "writer time: " << writerTime / (double)CLOCKS_PER_SEC << std::endl;
      std::cout << "File finished" << std::endl;
   }
}
