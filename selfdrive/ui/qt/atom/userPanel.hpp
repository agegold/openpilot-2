#pragma once



#include <QWidget>
#include <QFrame>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QScrollArea>
#include <QStackedWidget>

#include "selfdrive/ui/qt/widgets/controls.hpp"


class CUserPanel : public QFrame 
{
  Q_OBJECT
public:
  explicit CUserPanel(QWidget* parent = nullptr);

private:
   
protected:
  void showEvent(QShowEvent *event) override;

};





class CAutoResumeToggle : public ToggleControl {
  Q_OBJECT

public:
  CAutoResumeToggle() : ToggleControl("자동출발 기능 사용", "SCC 사용중 정차시 자동출발 기능을 사용합니다.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrAutoResume")) {
    QObject::connect(this, &CAutoResumeToggle::toggleFlipped, [=](int state) {
      Params().putBool("OpkrAutoResume", (bool)state);
    });
  }
};


class CLiveSteerRatioToggle : public ToggleControl {
  Q_OBJECT

public:
  CLiveSteerRatioToggle() : ToggleControl("Live SteerRatio 사용", "가변/고정 SR 대신 Live SteerRatio를 사용합니다.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrLiveSteerRatio")) {
    QObject::connect(this, &CLiveSteerRatioToggle::toggleFlipped, [=](int state) {
      Params().putBool("OpkrLiveSteerRatio", (bool)state);
    });
  }
};


class CTurnSteeringDisableToggle : public ToggleControl {
  Q_OBJECT

public:
  CTurnSteeringDisableToggle() : ToggleControl("턴시그널 사용시 조향해제 사용", "차선변경속도 이하로 주행할 때 턴시그널을 사용시 자동조향을 일시해제 합니다.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrTurnSteeringDisable")) {
    QObject::connect(this, &CTurnSteeringDisableToggle::toggleFlipped, [=](int state) {
      Params().putBool("OpkrTurnSteeringDisable", (bool)state);
    });
  }
};


class CPrebuiltToggle : public ToggleControl {
  Q_OBJECT

public:
  CPrebuiltToggle() : ToggleControl("Prebuilt 파일 생성", "Prebuilt 파일을 생성하며 부팅속도를 단축시킵니다. UI수정을 한 경우 기능을 끄십시오.", "../assets/offroad/icon_shell.png", Params().getBool("PutPrebuiltOn")) {
    QObject::connect(this, &CPrebuiltToggle::toggleFlipped, [=](int state) {
      Params().putBool("OpkrPrebuilt", (bool)state);
    });
  }
};


class BrightnessControl : public AbstractControl 
{
  Q_OBJECT

public:
  BrightnessControl();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};


class AutoScreenOff : public AbstractControl {
  Q_OBJECT

public:
  AutoScreenOff();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;

  void refresh();
};
