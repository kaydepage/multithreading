   /**
    * startup code provided by Paul Miller for COSC1114 - Operating Systems
    * Principles
    **/
   #include "reader.h"

   #include "writer.h"

   /**
    * implement the functions needed for this class
    **/
   std::ifstream reader::in;
   pthread_t reader::readerThreadArray[MAX_THREADS];
   pthread_mutex_t reader::getLineMutex;
   bool reader::useTiming{false};
   double reader::readerUptime;
   double reader::readerLockTiming;
   double reader::readerTiming;
   double reader::readerWaitTiming;
   int reader::lockCount;
   int reader::readCount;

   reader::reader(int threads) : numThreads(threads){}

   //not sure if a mutex is needed since init should only be called once?
   void reader::init(const std::string& name) {
      if (!in.is_open()) {
         reader::in.open(name);
      }
      pthread_mutex_init(&getLineMutex, NULL);
   }
   //deconstructor
   reader::~reader() {
      if (useTiming) {
         std::cout << "Average Reader Thread Uptime Before Lock: " << readerUptime / numThreads / (double)CLOCKS_PER_SEC << std::endl;
         std::cout << "Average Reader Thread Uptime After Lock: " << readerTiming / numThreads / (double)CLOCKS_PER_SEC << std::endl;
         std::cout << "Average Read Time Per Line: " << readerUptime / readCount / (double)CLOCKS_PER_SEC << std::endl;
         std::cout << "Average Read Waiting Time: " << readerWaitTiming / numThreads / (double)CLOCKS_PER_SEC << std::endl;
         std::cout << "---------------" << std::endl;
         std::cout << "Average Reader Lock Time:" << readerLockTiming / lockCount / (double)CLOCKS_PER_SEC << std::endl;
         std::cout << "---------------" << std::endl;
      }   
      pthread_mutex_destroy(&getLineMutex);

   }

   //creation and join of threads
   void reader::run() {
      for (int i{0}; i < numThreads; ++i) {
         int readerCreateResult = pthread_create(&readerThreadArray[i], 
            NULL, 
            reader::runner, 
            static_cast<void*>(&getLineMutex));
         

         if (readerCreateResult != 0) {
            std::cerr << "Reader create fail on thread "<< i << " Result: " << readerCreateResult << std::endl;
         }
      }
   }

   //the reader threads pulling from the txt and adding to queue?
   //needs a mutex to lock pulling from the txt file
   void* reader::runner(void* arg) {
      clock_t readerStart;
      clock_t readerEnd;
      clock_t lockEnd;
      clock_t waitEnd;
      bool looping{true};
      pthread_mutex_t *getLineMutex = static_cast<pthread_mutex_t*>(arg);
      
      while (looping) {
         if (useTiming) {
            readerStart = clock();
         }
         pthread_mutex_lock(getLineMutex);
         if (useTiming) {
            lockEnd = clock();
            readerLockTiming += (static_cast<double>(lockEnd - readerStart));
            lockCount += 1;
         }
         std::string input{};
         while (writer::queue.size() >= MAX_QUEUE_SIZE) {
            pthread_cond_wait(&writer::queueNotFull, getLineMutex);
         }
         if (useTiming) {
            waitEnd = clock();
            readerWaitTiming += (static_cast<double>(waitEnd - lockEnd));
         }
         if(std::getline(in, input)) {
            writer::append(input);
            pthread_cond_signal(&writer::queueNotEmpty);
         }
         else {
               writer::setfinished();
               looping = false;
         }
         
         
         pthread_mutex_unlock(getLineMutex);
         if (useTiming) {
            readerEnd = clock();
            readerUptime += static_cast<double>(readerEnd - readerStart);
            readerTiming += static_cast<double>(readerEnd - lockEnd);
            readCount += 1;
         }
      }
      pthread_cond_signal(&writer::queueNotEmpty);
      return nullptr;
      
   }
