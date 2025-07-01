/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include <pthread.h>
#include <time.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#define MAX_THREADS 100
#ifndef WRITER
#define WRITER
class writer {
   public:
    /**
     * Please note that methods and data need to be static as there's a
     * variety of information that needs to be coordinated between writers
     * such as locks and other shared data. Also, as the pthread library
     * is a c library, it does not know about classes so ruuner in particular
     * needs to be static. You can pass in instances into the function as
     * pointers though.
     **/
    writer(int threads);
    //initialising the queue and the txt file to output?
    static void init(const std::string& name);
    //deconstructor
    ~writer();
    //the writing threads pulling from queue and outputing
    static void* runner(void*);
    //creating the writer threads
    void run();
    //called by reader to add to the queue
    static void append(const std::string& line);
    //called by reader to indicate to queue to stop waiting for lines.
    static void setfinished();

   static pthread_t writerThreadArray[MAX_THREADS];
   static pthread_cond_t queueNotEmpty;
   static std::deque<std::string> queue;
   static pthread_cond_t queueNotFull;

   static bool useTiming;
   static double appendTiming;
   static double writerUptime;
   static double writerLockTiming;
   static double writerTiming;
   static double writerWaitTiming;
   static int lockCount;
   static int writeCount;


   private:
    int numThreads;
    static bool finishedReading;
    static std::ofstream out;
    
    static pthread_mutex_t queueMutex;
    static pthread_mutex_t writerMutex;
    
    
};
#endif
