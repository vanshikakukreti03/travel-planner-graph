FROM gcc:13

WORKDIR /app

COPY . .

RUN g++ -std=c++17 -O2 -Wall -Wextra -Iinclude \
    src/main.cpp src/Graph.cpp src/Traversal.cpp src/Dijkstra.cpp \
    -pthread -o travel_planner

EXPOSE 8080

CMD ["./travel_planner"]