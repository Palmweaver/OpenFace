// -*- c++ -*-
#ifndef OPENFACEWIDGET_H
#define OPENFACEWIDGET_H

#include <map>

#include <boost/format.hpp>

#include <QPixmap>
#include <QWidget>
#include <QGraphicsView>
#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QSplitter>
#include <QVBoxLayout>
#include <QLabel>

#include "render-thread.h"
#include "subprocess-worker-thread.h"

class OpenfaceWidget : public QMainWindow {
  Q_OBJECT
public:
  OpenfaceWidget(QWidget *parent = 0);

signals:
  void do_classify_parameters(std::string &);

private slots:
  void ml_program_result(std::string &);
  void updatePixmap(const QImage &image, double scaleFactor);
  void update_action_units_display(std::map<std::string, std::pair<bool, double>> turned_on_action_units);

private:
  SubprocessWorkerThread m_subprocess_worker_thread;
  RenderThread m_thread;
  QGraphicsPixmapItem m_pixmap;
  QWidget m_information_container;
  QSplitter m_main_app_splitter;
  QGraphicsView m_graphics_view;
  QVBoxLayout m_action_unit_layout;
  QLabel m_process_result;
  std::map<std::string, QWidget*> m_action_unit_collection;

  boost::format action_unit_format_string;
  boost::format m_process_result_format_string;

  void setup_action_unit_display(void);
};

#endif
