/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include <pthread.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <string>
#define MAX_QUEUE_SIZE 2000
#define MAX_THREADS 100
#ifndef READER
#define READER


class reader {
   public:
    /* this class has a bunch of static (which means shared in a class)
     * because we need to share the state between threads. For example, we
     * are reading from the same file, should have the same locks for all
     * readers, etc.
     **/
    reader(int threads);
    /** initialize the shared data for the class - for example, open the
     * file. There will be other things which you will need to figure out
     * as you complete the assignment.
     **/
    static void init(const std::string& name);
    ~reader();
    /**
     * the method that implements the thread. It has to be static as the first
     * implicit argument to all instance methods is 'this' and c functions don't
     * know about classes
     **/
    static void* runner(void*);

    /**
     * does the setup for and launches the threads
     **/
    void run();
    /**
     * there may be other functions you need so declare them.
     **/
    static pthread_t readerThreadArray[MAX_THREADS];

    static double readerUptime;
    static bool useTiming;
    static double readerLockTiming;
    static double readerTiming;
    static double readerWaitTiming;

    static int lockCount;
    static int readCount;
   private:
    
    int numThreads;
    static std::ifstream in;
    
    /**
     * There may be other private instance data you need so declare those here.
     **/
    //array used to store pthread ids for use in joining.
    static pthread_mutex_t getLineMutex;
};
#endif
