   /**
    * startup code provided by Paul Miller for COSC1114 - Operating Systems
    * Principles
    **/
   #include "writer.h"

   #include "reader.h"

   bool writer::finishedReading = false;
   std::ofstream writer::out;
   std::deque<std::string> writer::queue;
   pthread_t writer::writerThreadArray[MAX_THREADS];
   pthread_mutex_t writer::queueMutex;
   pthread_mutex_t writer::writerMutex;
   pthread_cond_t writer::queueNotEmpty;
   pthread_cond_t writer::queueNotFull;
   bool writer::useTiming{false};
   double writer::appendTiming;
   double writer::writerUptime;
   double writer::writerLockTiming;
   double writer::writerTiming;
   double writer::writerWaitTiming;
   int writer::lockCount{0};
   int writer::writeCount{0};


   writer::writer(int threads) : numThreads(threads){}
   /**
    * implement these functions requred for the writer class
    **/
   void writer::init(const std::string& name) {
      out.open(name.c_str(), std::ios::out | std::ios::trunc);
      pthread_mutex_init(&queueMutex, NULL);
      pthread_mutex_init(&writerMutex, NULL);
      pthread_cond_init(&queueNotEmpty, NULL);
      pthread_cond_init(&queueNotFull, NULL);
   }
   //deconstructor
   writer::~writer() {
      if (useTiming) {
         std::cout << "Average Writer Thread Uptime Before Lock: " << writerUptime / numThreads / (double)CLOCKS_PER_SEC << std::endl;
         std::cout << "Average Writer Thread Uptime After Lock: " << writerTiming / numThreads / (double)CLOCKS_PER_SEC << std::endl;
         std::cout << "Average Write Time Per Line: " << writerUptime / writeCount / (double)CLOCKS_PER_SEC << std::endl;
         std::cout << "Average Writer Wait Time: " << writerWaitTiming / numThreads / (double)CLOCKS_PER_SEC << std::endl;
         std::cout << "---------------" << std::endl;
         std::cout << "Average Writer Lock Time:" << writerLockTiming / lockCount / (double)CLOCKS_PER_SEC << std::endl;
         std::cout << "---------------" << std::endl;
         std::cout << "Time Spent Appending: " << appendTiming / (double)CLOCKS_PER_SEC << std::endl;
         std::cout << "Average Appent Time Per Line: " << appendTiming / writeCount / (double)CLOCKS_PER_SEC << std::endl;
         std::cout << "---------------" << std::endl;
      }
      pthread_mutex_destroy(&queueMutex);
      pthread_mutex_destroy(&writerMutex);
      pthread_cond_destroy(&queueNotEmpty);
      pthread_cond_destroy(&queueNotFull);
      out.close();

      
   }
   //construction of the writer threads and joining of threads
   void writer::run() {
      for (int i{0}; i < numThreads; ++i) {
         int writerCreateResult = pthread_create(&writerThreadArray[i], 
            NULL, 
            writer::runner, 
            static_cast<void*>(&writerMutex));
         if (writerCreateResult != 0) {
            std::cerr << "Writer create fail on thread "<< i << " Result: " << writerCreateResult << std::endl;
         }
      }
   }
   //the pulling of items from the queue and writing them into outstream out which is linked to a text file specified in init
   void* writer::runner(void* arg) { 
      clock_t writerStart;
      clock_t writerEnd;
      clock_t lockEnd;  
      clock_t waitEnd;    
      pthread_mutex_t *writerMutex = static_cast<pthread_mutex_t*>(arg);
      bool looping{true};
      while (looping) {
         if (useTiming) {
            writerStart = clock();
         }
         //std::cout << finishedReading << std::endl;
         pthread_mutex_lock(writerMutex);
         if (useTiming) {
            lockEnd = clock();
            writerLockTiming += (static_cast<double>(lockEnd - writerStart));
            lockCount += 1;
         }
         while (queue.empty() && !finishedReading) {
            //std::cout << "waiting" << std::endl;
            pthread_cond_wait(&queueNotEmpty, writerMutex);
         }
         if (useTiming) {
            waitEnd = clock();
            writerWaitTiming = static_cast<double>(waitEnd - lockEnd);
         }
         pthread_mutex_lock(&queueMutex);
         if (!queue.empty()) {
            out << queue.front() << std::endl;
            queue.pop_front();
            pthread_cond_signal(&queueNotFull);
         }
         pthread_mutex_unlock(&queueMutex);

         if (queue.empty() && finishedReading) {
            looping = false;
         }
         pthread_mutex_unlock(writerMutex);
         if (useTiming) {
            writerEnd = clock();
            writerUptime += static_cast<double>(writerEnd - writerStart);
            writerTiming += static_cast<double>(writerEnd - lockEnd);
            writeCount += 1;
         }
      }
      return nullptr;
   }

   //used after reader threads read a line from the input textfile.
   void writer::append(const std::string& line) {
      clock_t appendStart;
      clock_t appendEnd;
      if (useTiming) {
         appendStart = clock();
      }
      pthread_mutex_lock(&queueMutex);
      queue.push_back(line);
      pthread_mutex_unlock(&queueMutex);
      if (useTiming) {
         appendEnd = clock();
         appendTiming += static_cast<double>(appendEnd - appendStart);
      }
   }
   //?? when the readers are finished 
   void writer::setfinished() {
      if (finishedReading == false) {
         finishedReading = true;
      } 
   }