#include "server/server.h"
#include "algorithms/algorithm_mock/algorithm_mock.h"
#include "workloads/workload_static/workload_static.h"
#include "sender/sender.h"

#include <iostream>

int main() {
    AlgorithmMock algorithm;
    Server server(8000, &algorithm);
    server.start();
    WorkloadStatic workload(3, 100);
    Sender sender(&workload, 8000);
    sender.start_execution();
    return 0;
}
