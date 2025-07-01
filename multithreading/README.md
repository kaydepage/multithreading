To run the program for either single threaded or multithreaded version:
    In the folder with the makefile, to create the single threaded executible:
        make copier
    To create the multithreaded executible:
        make mtcopier
    To create both executibles:
        make all
    To remove all executibles:
        make clean

    Single threaded:
        Inside the executible's location:
            ./copier <input> <output>

    Multithreaded:
        Inside the executible's location:
            ./mtcopier <number of threads> <input> <output> <include "-t" to use timing, ignore if not using timing>