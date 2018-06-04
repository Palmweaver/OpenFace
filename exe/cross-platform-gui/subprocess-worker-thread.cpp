#include <iostream>

#include "subprocess-worker-thread.h"
#include <process.hpp>

SubprocessWorkerThread::SubprocessWorkerThread(QObject *parent)
    : QThread(parent) {}

SubprocessWorkerThread::~SubprocessWorkerThread() {
  mutex.lock();
  abort = true;
  condition.wakeOne();
  mutex.unlock();
  wait();
}

void SubprocessWorkerThread::run() {
  forever {}
}

void SubprocessWorkerThread::do_classify_parameters(std::string &f) {
  using namespace TinyProcessLib;


  Process process2("./program.py", "",
  				   nullptr,
                   [this](const char *bytes, size_t n) {
  					 std::string msg{"Hello Reply"};
  					 // emit processed_results(msg);
  					 std::string reply{bytes, n};
  					 std::cout << reply;
                   });
}
