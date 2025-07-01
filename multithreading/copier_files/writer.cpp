/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/

#include "writer.h"

/**
 * provide your implementation for the writer functions here
 **/
writer::writer(const std::string& name) : out(name.c_str()) {}

void writer::run() {
   while (!queue.empty()) {
      out << queue.front() << "\n";
      queue.pop_front();
   }
   //std::cout << "Queue emptied and file filled." << std::endl;
}

void writer::append(const std::string& line) {
   queue.push_back(line);

}
