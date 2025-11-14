CXX = g++
CXXFLAGS = -Wall -std=c++17 -pthread
LIBS = -lsqlite3

SRV_SRC = main.cpp server.cpp utils.cpp thread_pool.cpp semaphore.cpp shared_memory.cpp shared_queue.cpp controller.cpp service.cpp repository.cpp database.cpp http_request.cpp http_response.cpp
SRV_OBJ = $(SRV_SRC:.cpp=.o)

WORKER_SRC = worker.cpp thread_pool.cpp semaphore.cpp shared_memory.cpp shared_queue.cpp utils.cpp controller.cpp service.cpp repository.cpp database.cpp http_request.cpp http_response.cpp
WORKER_OBJ = $(WORKER_SRC:.cpp=.o)

all: server worker

server: $(SRV_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

worker: $(WORKER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f *.o server worker