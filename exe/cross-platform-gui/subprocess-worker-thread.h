// -*- c++ -*-
#ifndef SUBPROCESS_WORKER_THREAD_H
#define SUBPROCESS_WORKER_THREAD_H

#include <QMutex>
#include <QObject>
#include <QSize>
#include <QThread>
#include <QWaitCondition>

class SubprocessWorkerThread : public QThread {
  Q_OBJECT
public:
  SubprocessWorkerThread(QObject *parent = 0);
  ~SubprocessWorkerThread();

signals:
  void processed_results(std::string &);
protected:
  void run() override;
public slots:
  void do_classify_parameters(std::string &);

private:
  QMutex mutex;
  QWaitCondition condition;
  QSize resultSize;

  bool restart;
  bool abort;
};

#endif
