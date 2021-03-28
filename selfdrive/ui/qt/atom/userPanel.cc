
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>



#include "widgets/input.hpp"
#include "widgets/toggle.hpp"
#include "widgets/offroad_alerts.hpp"
#include "widgets/controls.hpp"
#include "widgets/ssh_keys.hpp"
#include "common/params.h"
#include "common/util.h"


#include "userPanel.hpp"


UserPanel::UserPanel(QWidget* parent) : QWidget(parent)
{
  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(100);
  layout->setSpacing(30);

  // simple wifi + tethering buttons
  const char* launch_wifi = "am start -n com.android.settings/.wifi.WifiPickerActivity \
                             -a android.net.wifi.PICK_WIFI_NETWORK \
                             --ez extra_prefs_show_button_bar true \
                             --es extra_prefs_set_next_text ''";
  layout->addWidget(new ButtonControl("WiFi Settings", "OPEN", "",
                                      [=]() { std::system(launch_wifi); }));

  layout->addWidget(horizontal_line());

  const char* launch_tethering = "am start -n com.android.settings/.TetherSettings \
                                  --ez extra_prefs_show_button_bar true \
                                  --es extra_prefs_set_next_text ''";
  layout->addWidget(new ButtonControl("Tethering Settings", "OPEN", "",
                                      [=]() { std::system(launch_tethering); }));

  layout->addWidget(horizontal_line());

  // SSH key management
  layout->addWidget(new SshToggle());
  layout->addWidget(horizontal_line());
  layout->addWidget(new SshControl());

  layout->addStretch(1);

  QWidget *w = new QWidget;
  w->setLayout(layout);
}


UserPanel::UserPanel(QWidget* parent) : QFrame(parent) 
{
  QVBoxLayout *main_layout = new QVBoxLayout(this);
  main_layout->setMargin(100);
  setLayout(main_layout);
  setStyleSheet(R"(QLabel {font-size: 50px;})");
}

void UserPanel::showEvent(QShowEvent *event) 
{
  Params params = Params();
  std::string brand = params.read_db_bool("Passive") ? "dashcam" : "openpilot";
  QList<QPair<QString, std::string>> dev_params = {
    {"Version", brand + " v" + params.get("Version", false).substr(0, 14)},
    {"Git Branch", params.get("GitBranch", false)},
    {"Git Commit", params.get("GitCommit", false).substr(0, 10)},
    {"Panda Firmware", params.get("PandaFirmwareHex", false)},
    {"OS Version", Hardware::get_os_version()},
  };

  for (int i = 0; i < dev_params.size(); i++) {
    const auto &[name, value] = dev_params[i];
    QString val = QString::fromStdString(value).trimmed();
    if (labels.size() > i) {
      labels[i]->setText(val);
    } else {
      labels.push_back(new LabelControl(name, val));
      layout()->addWidget(labels[i]);
      if (i < (dev_params.size() - 1)) {
        layout()->addWidget(horizontal_line());
      }
    }
  }

  layout()->addWidget(new SshToggle());
}

/*
QWidget * user_panel(QWidget * parent) 
{
  QVBoxLayout *layout = new QVBoxLayout;

  layout->setMargin(100);
  layout->setSpacing(30);

  // OPKR
  std::vector<std::pair<std::string, std::string>> labels1 = {{"토글메뉴(UI)", ""},};
  for (auto &l : labels1) {layout->addWidget(new LabelControl(QString::fromStdString(l.first),
                             QString::fromStdString(l.second)));
  }
  layout->addWidget(new GetoffAlertToggle());
  layout->addWidget(new BatteryChargingControlToggle());
  layout->addWidget(new DrivingRecordToggle());
  layout->addWidget(new HotspotOnBootToggle());

  layout->addWidget(horizontal_line());
  std::vector<std::pair<std::string, std::string>> labels2 = {{"토글메뉴(주행)", ""},};
  for (auto &l : labels2) {layout->addWidget(new LabelControl(QString::fromStdString(l.first),
                             QString::fromStdString(l.second)));
  }
  layout->addWidget(new AutoResumeToggle());
  layout->addWidget(new VariableCruiseToggle());
  layout->addWidget(new BlindSpotDetectToggle());
  layout->addWidget(new TurnSteeringDisableToggle());
  layout->addWidget(new CruiseOverMaxSpeedToggle());
  layout->addWidget(new MapDecelOnlyToggle());

  layout->addWidget(horizontal_line());
  std::vector<std::pair<std::string, std::string>> labels3 = {{"토글메뉴(개발자)", ""},};
  for (auto &l : labels3) {layout->addWidget(new LabelControl(QString::fromStdString(l.first),
                             QString::fromStdString(l.second)));
  }
  layout->addWidget(new DebugUiOneToggle());
  layout->addWidget(new DebugUiTwoToggle());
  layout->addWidget(new PrebuiltToggle());
  layout->addWidget(new FPToggle());
  layout->addWidget(new FPTwoToggle());
  layout->addWidget(new LDWSToggle());
  //layout->addWidget(horizontal_line());

  layout->addStretch(1);

  QWidget *w = new QWidget;
  w->setLayout(layout);

  return w;
}
*/