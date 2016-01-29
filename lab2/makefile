all: server stat sigint

server: server.cpp
	g++ -o server server.cpp

stat: stat.cpp
	g++ -o stat stat.cpp

sigint: sigint.cpp
	g++ -o sigint sigint.cpp

run: server
	./server 3300 ~/htdocs/lab2/content

