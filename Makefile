CXX = g++
CXXFLAGS = -Wall -std=c++17 -pthread
LDFLAGS = -lsqlite3

SERVER_OBJS = main.o server.o controller.o service.o repository.o database.o \
http_response.o utils.o thread_pool.o semaphore.o shared_queue.o

WORKER_OBJS = worker.o controller.o service.o repository.o database.o \
http_response.o utils.o thread_pool.o semaphore.o shared_queue.o

all: server worker

server: $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o server $(SERVER_OBJS) $(LDFLAGS)

worker: $(WORKER_OBJS)
	$(CXX) $(CXXFLAGS) -o worker $(WORKER_OBJS) $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

server.o: server.cpp
	$(CXX) $(CXXFLAGS) -c server.cpp

worker.o: worker.cpp
	$(CXX) $(CXXFLAGS) -c worker.cpp

controller.o: controller.cpp
	$(CXX) $(CXXFLAGS) -c controller.cpp

service.o: service.cpp
	$(CXX) $(CXXFLAGS) -c service.cpp

repository.o: repository.cpp
	$(CXX) $(CXXFLAGS) -c repository.cpp

database.o: database.cpp
	$(CXX) $(CXXFLAGS) -c database.cpp

http_response.o: http_response.cpp
	$(CXX) $(CXXFLAGS) -c http_response.cpp

utils.o: utils.cpp
	$(CXX) $(CXXFLAGS) -c utils.cpp

thread_pool.o: thread_pool.cpp
	$(CXX) $(CXXFLAGS) -c thread_pool.cpp

semaphore.o: semaphore.cpp
	$(CXX) $(CXXFLAGS) -c semaphore.cpp

shared_queue.o: shared_queue.cpp
	$(CXX) $(CXXFLAGS) -c shared_queue.cpp

clean:
	rm -f *.o server worker
