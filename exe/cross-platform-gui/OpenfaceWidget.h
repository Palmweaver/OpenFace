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

#include "render-thread.h"

class OpenfaceWidget : public QMainWindow {
  Q_OBJECT
public:
  OpenfaceWidget(QWidget *parent = 0);

protected:

private slots:
  void updatePixmap(const QImage &image, double scaleFactor);
  void update_action_units_display(std::map<std::string, bool> turned_on_action_units);

private:
  RenderThread m_thread;
  QGraphicsPixmapItem m_pixmap;
  QWidget m_information_container;
  QSplitter m_main_app_splitter;
  QGraphicsView m_graphics_view;
  QVBoxLayout m_action_unit_layout;
  std::map<std::string, QWidget*> m_action_unit_collection;

  boost::format action_unit_format_string;

  void setup_action_unit_display(void);
};

#endif
