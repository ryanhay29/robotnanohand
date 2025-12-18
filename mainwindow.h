#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSlider>
#include <QMap>
#include "hand.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct ServoConfig {
    int id;
    int min;
    int max;
    bool inverted;

    ServoConfig() {} // default
    ServoConfig(int _id, int _min, int _max, bool _inverted)
        : id(_id), min(_min), max(_max), inverted(_inverted) {}
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void setHandPose(const QVector<int> &positions);
    ~MainWindow();

private slots:
    void on_slider_valueChanged(int value);

    void on_resetButton_clicked();

    void on_peaceButton_clicked();

    void on_rockButton_clicked();

    void on_fistButton_clicked();

    void on_thumbButton_clicked();

    void on_ilyButton_clicked();

    void on_okButton_clicked();

    void on_bangbangButton_clicked();

    void on_oneButton_clicked();

    void on_twoButton_clicked();

    void on_threeButton_clicked();

    void on_fourButton_clicked();

    void on_fiveButton_clicked();

    void on_vulcanButton_clicked();

    void on_lButton_clicked();

private:
    Ui::MainWindow* ui;
    Hand hand;

    QVector<ServoConfig> servoConfigs;          // Maps servo index → servo ID & min/max
    QMap<QSlider*, int> sliderToServoIndex;     // Maps slider → servo index
};

#endif // MAINWINDOW_H
