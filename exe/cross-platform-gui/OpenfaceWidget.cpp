#include <iostream>
#include <sstream>

#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/map.hpp>

#include <QApplication>
#include <QLabel>
#include <QTextEdit>
#include <QTimer>

#include "OpenfaceWidget.h"

static std::map<std::string, std::string> action_unit_name = {
    {"AU01", "Inner Brow Raiser"},
    {"AU02", "Outer Brow Raiser"},
    {"AU04", "Brow Lowerer"},
    {"AU05", "Upper Lid Raiser"},
    {"AU06", "Cheek Raiser"},
    {"AU07", "Lid Tightener"},
    {"AU09", "Nose Wrinkler"},
    {"AU10", "Upper Lip Raiser"},
    {"AU12", "Lip Corner Puller"},
    {"AU14", "Dimpler"},
    {"AU15", "Lip Corner Depressor"},
    {"AU17", "Chin Raiser"},
    {"AU20", "Lip stretcher"},
    {"AU23", "Lip Tightener"},
    {"AU25", "Lips part"},
    {"AU26", "Jaw Drop"},
    {"AU28", "Lip Suck"},
    {"AU45", "Blink"},
};

OpenfaceWidget::OpenfaceWidget(QWidget *parent)
    : QMainWindow(parent), m_main_app_splitter(this),
      action_unit_format_string("Action Unit:%s (%s) %s") {

  QTimer *timer_worker = new QTimer{this};
  QObject::connect(timer_worker, &QTimer::timeout, &m_thread,
                   &RenderThread::do_csv_work);

  timer_worker->start(2000);

  QObject::connect(&m_thread, &RenderThread::action_units_produced, this,
                   &OpenfaceWidget::update_action_units_display);

  QObject::connect(&m_thread, &RenderThread::renderedImage, this,
                   &OpenfaceWidget::updatePixmap);
  QObject::connect(&m_thread, &QThread::finished, this, &QObject::deleteLater);

  m_information_container.setLayout(&m_action_unit_layout);

  // Two main views
  m_main_app_splitter.addWidget(&m_graphics_view);
  m_main_app_splitter.addWidget(&m_information_container);

  m_graphics_view.setScene(new QGraphicsScene(this));
  m_graphics_view.scene()->addItem(&m_pixmap);
  m_graphics_view.setMinimumWidth(950);
  setCentralWidget(&m_main_app_splitter);
  setWindowTitle(tr("OpenfaceWidget"));
  resize(1280, 720);

  setup_action_unit_display();
  m_thread.start();
}

void OpenfaceWidget::setup_action_unit_display(void) {
  std::vector<std::string> names;
  boost::copy(action_unit_name | boost::adaptors::map_keys, std::back_inserter(names));

  for (auto &action_unit: names) {
    m_action_unit_collection[action_unit] = new QLabel{"0", this};
  }

  for (auto &pair : m_action_unit_collection) {
    m_action_unit_layout.addWidget(pair.second);
  }
}

void OpenfaceWidget::updatePixmap(const QImage &qimg, double scaleFactor) {
  m_pixmap.setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
  m_graphics_view.fitInView(&m_pixmap, Qt::KeepAspectRatio);
  update();
}

void OpenfaceWidget::update_action_units_display(
    std::map<std::string, bool> turned_on_action_units) {

  for (auto &pair : turned_on_action_units) {
    std::stringstream ss;
    QLabel *label =
        static_cast<QLabel *>(m_action_unit_collection[pair.first]);
    if (pair.second == false) {
      label->setStyleSheet("background-color:red;");
      ss << action_unit_format_string % pair.first % action_unit_name[pair.first] %
                " is not turned on";
    } else {
      label->setStyleSheet("background-color:green;");
      ss << (action_unit_format_string % pair.first % action_unit_name[pair.first] %
             " is working");
    }

    label->setText(QString::fromStdString(ss.str()));
  }
}
