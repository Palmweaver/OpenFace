#include <iostream>
#include <sstream>

#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm.hpp>

#include <QApplication>
#include <QLabel>
#include <QTextEdit>
#include <QTimer>

#include <process.hpp>

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
    // {"AU28", "Lip Suck"},
    {"AU45", "Blink"},
};

OpenfaceWidget::OpenfaceWidget(QWidget *parent)
    : QMainWindow(parent), m_main_app_splitter(this),
      m_process_result("To be Replaced", this),
      m_process_result_format_string("Prediction of face: %s %f confidence"),
      action_unit_format_string("Action Unit:%s (%s) %.2f") {
  QTimer *timer_worker = new QTimer{this};

  QTimer *ml_worker_timer = new QTimer{this};

  ml_worker_timer->start(1000);
  double c = 1.0;
  QObject::connect(timer_worker, &QTimer::timeout, [this, &c]() {
    using namespace TinyProcessLib;
    Process process2("./program.py", "", nullptr,
                     [this](const char *bytes, size_t n) {
                       // std::stringstream ss;
                       // ss << m_process_result_format_string % "Result" % c++;
                       // std::cout << "Second timer called" << ss.str() << "\n";
                       // emit processed_results(msg);
                       std::string reply{bytes, n};
					   m_process_result.setText(QString::fromStdString(reply));
                     });
  });

  QObject::connect(timer_worker, &QTimer::timeout, &m_thread,
                   &RenderThread::do_csv_work);

  // Worker thread just for processing image data
  QObject::connect(&m_thread, &RenderThread::action_units_produced, this,
                   &OpenfaceWidget::update_action_units_display);
  QObject::connect(&m_thread, &RenderThread::renderedImage, this,
                   &OpenfaceWidget::updatePixmap);

  QObject::connect(&m_thread, &QThread::finished, this, &QObject::deleteLater);

  // Python worker ML process
  QObject::connect(this, &OpenfaceWidget::do_classify_parameters,
                   &m_subprocess_worker_thread,
                   &SubprocessWorkerThread::do_classify_parameters);
  QObject::connect(&m_subprocess_worker_thread,
                   &SubprocessWorkerThread::processed_results, this,
                   &OpenfaceWidget::ml_program_result);

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
  m_action_unit_layout.addWidget(&m_process_result);
  m_thread.start();
  // m_subprocess_worker_thread.start();
  timer_worker->start(1500);
}

void OpenfaceWidget::setup_action_unit_display(void) {
  std::vector<std::string> names;
  boost::copy(action_unit_name | boost::adaptors::map_keys,
              std::back_inserter(names));

  for (auto &action_unit : names) {
    m_action_unit_collection[action_unit] = new QLabel{"0", this};
  }

  m_action_unit_layout.addWidget(
      new QLabel{"Red means OpenFace does not think this feature exists, "
                 "intensity range is 0-5",
                 this});

  for (auto &pair : m_action_unit_collection) {
    m_action_unit_layout.addWidget(pair.second);
  }
}

void OpenfaceWidget::updatePixmap(const QImage &qimg, double scaleFactor) {
  m_pixmap.setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
  m_graphics_view.fitInView(&m_pixmap, Qt::KeepAspectRatio);
}

void OpenfaceWidget::ml_program_result(std::string &) {
  //
}

void OpenfaceWidget::update_action_units_display(
    std::map<std::string, std::pair<bool, double>> turned_on_action_units) {

  for (auto &pair : turned_on_action_units) {
    std::stringstream ss;
    QLabel *label = static_cast<QLabel *>(m_action_unit_collection[pair.first]);
    if (pair.second.first == false) {
      label->setStyleSheet("background-color:red;");
      ss << action_unit_format_string % pair.first %
                action_unit_name[pair.first] % pair.second.second;
    } else {
      label->setStyleSheet("background-color:green;");
      ss << action_unit_format_string % pair.first %
                action_unit_name[pair.first] % pair.second.second;
    }

    label->setText(QString::fromStdString(ss.str()));
    std::string message{"Hello World"};
    // emit do_classify_parameters(message);
  }
}
