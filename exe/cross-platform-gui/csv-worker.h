// -*- c++ -*-
#ifndef CSV_WORKER_H
#define CSV_WORKER_H

#include <QThread>

class CsvWorker : public QThread {
  Q_OBJECT
public:
  CsvWorker(QObject *parent = 0);
  ~CsvWorker();
};

#endif
