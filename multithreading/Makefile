CC = g++
CXXFLAGS = -g -Wall -Werror -std=c++20
LDFLAGS = -lpthread

.PHONY: all clean

all: copier mtcopier

copier: copier_files/main.cpp copier_files/reader.cpp copier_files/writer.cpp
	$(CC) $(CXXFLAGS) -o copier_files/copier copier_files/main.cpp copier_files/reader.cpp copier_files/writer.cpp

mtcopier: mtcopier_files/main.cpp mtcopier_files/reader.cpp mtcopier_files/writer.cpp
	$(CC) $(CXXFLAGS) -o mtcopier_files/mtcopier $(LDFLAGS) mtcopier_files/main.cpp mtcopier_files/reader.cpp mtcopier_files/writer.cpp

clean:
	rm -f copier_files/copier mtcopier_files/mtcopier
