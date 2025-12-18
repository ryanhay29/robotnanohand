#include <QMessageBox>
#include <QApplication>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon.png"));
    this->setWindowTitle("Robot Hand Controller");

    // Setup servos
    bool connected = hand.setup_servos();

    //If the servos are not connected, the user cannot adjust the sliders and error message appears
    ui->centralwidget->setEnabled(connected);

    // Configure servos (ID, min, max, inverted)
    servoConfigs = {
        {1, 451, 547, true},  // Pinky Wiggle
        {2, 95, 825, true},    // Pinky Curl
        {3, 464, 530, true},  // Ring Wiggle
        {4, 115, 916, true},   // Ring Curl
        {5, 467, 547, true},  // Middle Wiggle
        {6, 165, 980, false},   // Middle Curl
        {7, 486, 553, true},  // Index Wiggle
        {8, 230, 927, false},   // Index Curl
        {9, 435, 790, true},  // Thumb Wiggle
        {10, 221, 857, true}   // Thumb Curl
    };

    // Map sliders to servo index
    sliderToServoIndex[ui->pinkyWiggle]  = 0;
    sliderToServoIndex[ui->pinkyCurl]    = 1;
    sliderToServoIndex[ui->ringWiggle]   = 2;
    sliderToServoIndex[ui->ringCurl]     = 3;
    sliderToServoIndex[ui->middleWiggle] = 4;
    sliderToServoIndex[ui->middleCurl]   = 5;
    sliderToServoIndex[ui->indexWiggle]  = 6;
    sliderToServoIndex[ui->indexCurl]    = 7;
    sliderToServoIndex[ui->thumbWiggle]  = 8;
    sliderToServoIndex[ui->thumbCurl]    = 9;

    // Initialize sliders
    on_resetButton_clicked();

    //Show program
    this->show();

    //Error message
    if (!connected) {
        // Create the message box as a **child of main window**
        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setWindowIcon(QIcon(":/icon.png"));
        msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
        msgBox->setIcon(QMessageBox::Critical);
        msgBox->setWindowTitle("Hand Not Connected");
        msgBox->setText(
            "HAND NOT CONNECTED\n\n"
            "Please connect the hand and restart the program."
            );
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->setDefaultButton(QMessageBox::Ok);

        // Connect the Ok button to quit the app
        QObject::connect(msgBox, &QMessageBox::finished, qApp, &QApplication::quit);
        msgBox->show();
    }
}

void MainWindow::on_resetButton_clicked()
{
    for (auto it = sliderToServoIndex.begin(); it != sliderToServoIndex.end(); it++) {
        QSlider* slider = it.key();
        int idx = it.value();
        ServoConfig cfg = servoConfigs[idx];

        slider->setMinimum(cfg.min);
        slider->setMaximum(cfg.max);

        connect(slider, &QSlider::valueChanged,
                this, &MainWindow::on_slider_valueChanged);

        int initialPos;
        if (slider->orientation() == Qt::Vertical) {
            // Vertical: top = fully extended
            if (cfg.inverted == true){
                initialPos = cfg.min;
            }
            else{
                initialPos = cfg.max;
            }


        } else {
            // Horizontal: centered
            initialPos = (cfg.min + cfg.max) / 2;
        }

        hand.Servos.WritePos(cfg.id, initialPos, 200); // move servo
        slider->setValue(initialPos);                  // update slider
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setHandPose(const QVector<int> &positions)
{
    if (positions.size() != servoConfigs.size()) {
        qWarning() << "Positions vector must have" << servoConfigs.size()
        << "elements, but has" << positions.size();
        return;
    }

    // Block slider signals while updating
    for (auto slider : sliderToServoIndex.keys()) {
        slider->blockSignals(true);
    }

    for (int i = 0; i < positions.size(); ++i) {
        const ServoConfig &cfg = servoConfigs[i];
        int targetPos = positions[i];

        // Clamp to servo min/max
        targetPos = qBound(cfg.min, targetPos, cfg.max);

        hand.Servos.WritePos(cfg.id, targetPos, 200); // Move servo

        // Update slider
        QSlider* slider = sliderToServoIndex.key(i);
        if (slider) {
            slider->setValue(targetPos);
        }
    }

    // Re-enable slider signals
    for (auto slider : sliderToServoIndex.keys()) {
        slider->blockSignals(false);
    }
}

void MainWindow::on_slider_valueChanged(int sliderValue)
{
    QSlider* slider = qobject_cast<QSlider*>(sender());
    if (!slider) return;

    int idx = sliderToServoIndex.value(slider, -1);
    if (idx < 0) return;

    ServoConfig cfg = servoConfigs[idx];

    int servoPos = sliderValue;

    hand.Servos.WritePos(cfg.id, servoPos, 100); // move servo

    // Update the corresponding label
    QLabel* label = nullptr;

    if (slider == ui->pinkyWiggle) label = ui->pinkyWiggleLabel;
    else if (slider == ui->pinkyCurl) label = ui->pinkyCurlLabel;
    else if (slider == ui->ringWiggle) label = ui->ringWiggleLabel;
    else if (slider == ui->ringCurl) label = ui->ringCurlLabel;
    else if (slider == ui->middleWiggle) label = ui->middleWiggleLabel;
    else if (slider == ui->middleCurl) label = ui->middleCurlLabel;
    else if (slider == ui->indexWiggle) label = ui->indexWiggleLabel;
    else if (slider == ui->indexCurl) label = ui->indexCurlLabel;
    else if (slider == ui->thumbWiggle) label = ui->thumbWiggleLabel;
    else if (slider == ui->thumbCurl) label = ui->thumbCurlLabel;

    if (label) {
        label->setText(QString::number(sliderValue));
    }
}

void MainWindow::on_peaceButton_clicked()
{
    QVector<int> peacePose = {
        544,  // Pinky Wiggle
        825,  // Pinky Curl
        530,  // Ring Wiggle
        916,  // Ring Curl
        526,  // Middle Wiggle
        980,  // Middle Curl
        493,  // Index Wiggle
        927,  // Index Curl
        456,  // Thumb Wiggle
        857   // Thumb Curl
    };
    setHandPose(peacePose);
}

void MainWindow::on_thumbButton_clicked()
{
    QVector<int> thumbPose = {
        499,  // Pinky Wiggle
        825,  // Pinky Curl
        497,  // Ring Wiggle
        916,  // Ring Curl
        507,  // Middle Wiggle
        165,  // Middle Curl
        519,  // Index Wiggle
        230,  // Index Curl
        435,  // Thumb Wiggle
        221   // Thumb Curl
    };
    setHandPose(thumbPose);
}

void MainWindow::on_rockButton_clicked()
{
    QVector<int> rockPose = {
        499,  // Pinky Wiggle
        95,  // Pinky Curl
        497,  // Ring Wiggle
        916,  // Ring Curl
        507,  // Middle Wiggle
        165,  // Middle Curl
        519,  // Index Wiggle
        927,  // Index Curl
        538,  // Thumb Wiggle
        807   // Thumb Curl
    };
    setHandPose(rockPose);
}

void MainWindow::on_fistButton_clicked()
{
    QVector<int> fistPose = {
        499,  // Pinky Wiggle
        825,  // Pinky Curl
        497,  // Ring Wiggle
        916,  // Ring Curl
        507,  // Middle Wiggle
        165,  // Middle Curl
        519,  // Index Wiggle
        230,  // Index Curl
        435,  // Thumb Wiggle
        708   // Thumb Curl
    };
    setHandPose(fistPose);
}

void MainWindow::on_ilyButton_clicked()
{
    QVector<int> ilyPose = {
        499,  // Pinky Wiggle
        95,  // Pinky Curl
        497,  // Ring Wiggle
        916,  // Ring Curl
        507,  // Middle Wiggle
        165,  // Middle Curl
        519,  // Index Wiggle
        927,  // Index Curl
        435,  // Thumb Wiggle
        221   // Thumb Curl
    };
    setHandPose(ilyPose);
}


void MainWindow::on_okButton_clicked()
{
    QVector<int> okPose = {
        547,  // Pinky Wiggle
        95,  // Pinky Curl
        523,  // Ring Wiggle
        115,  // Ring Curl
        472,  // Middle Wiggle
        980,  // Middle Curl
        486,  // Index Wiggle
        483,  // Index Curl
        790,  // Thumb Wiggle
        711   // Thumb Curl
    };
    setHandPose(okPose);
}


void MainWindow::on_bangbangButton_clicked()
{
    QVector<int> bangbangPose = {
        547,  // Pinky Wiggle
        825,  // Pinky Curl
        500,  // Ring Wiggle
        916,  // Ring Curl
        482,  // Middle Wiggle
        980,  // Middle Curl
        501,  // Index Wiggle
        927,  // Index Curl
        435,  // Thumb Wiggle
        221   // Thumb Curl
    };
    setHandPose(bangbangPose);
}


void MainWindow::on_oneButton_clicked()
{
    QVector<int> onePose = {
        499,  // Pinky Wiggle
        825,  // Pinky Curl
        497,  // Ring Wiggle
        916,  // Ring Curl
        507,  // Middle Wiggle
        165,  // Middle Curl
        519,  // Index Wiggle
        927,  // Index Curl
        612,  // Thumb Wiggle
        831   // Thumb Curl
    };
    setHandPose(onePose);
}


void MainWindow::on_twoButton_clicked()
{
    QVector<int> twoPose = {
        499,  // Pinky Wiggle
        825,  // Pinky Curl
        497,  // Ring Wiggle
        916,  // Ring Curl
        528,  // Middle Wiggle
        980,  // Middle Curl
        510,  // Index Wiggle
        927,  // Index Curl
        612,  // Thumb Wiggle
        831   // Thumb Curl
    };
    setHandPose(twoPose);
}


void MainWindow::on_threeButton_clicked()
{
    QVector<int> threePose = {
        499,  // Pinky Wiggle
        825,  // Pinky Curl
        521,  // Ring Wiggle
        115,  // Ring Curl
        528,  // Middle Wiggle
        980,  // Middle Curl
        510,  // Index Wiggle
        927,  // Index Curl
        612,  // Thumb Wiggle
        831   // Thumb Curl
    };
    setHandPose(threePose);
}


void MainWindow::on_fourButton_clicked()
{
    QVector<int> fourPose = {
        499,  // Pinky Wiggle
        95,  // Pinky Curl
        521,  // Ring Wiggle
        115,  // Ring Curl
        528,  // Middle Wiggle
        980,  // Middle Curl
        510,  // Index Wiggle
        927,  // Index Curl
        612,  // Thumb Wiggle
        831   // Thumb Curl
    };
    setHandPose(fourPose);
}


void MainWindow::on_fiveButton_clicked()
{
    QVector<int> fivePose = {
        499,  // Pinky Wiggle
        95,  // Pinky Curl
        521,  // Ring Wiggle
        115,  // Ring Curl
        528,  // Middle Wiggle
        980,  // Middle Curl
        510,  // Index Wiggle
        927,  // Index Curl
        435,  // Thumb Wiggle
        221   // Thumb Curl
    };
    setHandPose(fivePose);
}


void MainWindow::on_vulcanButton_clicked()
{
    QVector<int> vulcanPose = {
        481,  // Pinky Wiggle
        95,  // Pinky Curl
        530,  // Ring Wiggle
        115,  // Ring Curl
        467,  // Middle Wiggle
        980,  // Middle Curl
        486,  // Index Wiggle
        927,  // Index Curl
        612,  // Thumb Wiggle
        221   // Thumb Curl
    };
    setHandPose(vulcanPose);
}


void MainWindow::on_lButton_clicked()
{
    QVector<int> lPose = {
        481,  // Pinky Wiggle
        825,  // Pinky Curl
        530,  // Ring Wiggle
        916,  // Ring Curl
        467,  // Middle Wiggle
        165,  // Middle Curl
        486,  // Index Wiggle
        927,  // Index Curl
        435,  // Thumb Wiggle
        221   // Thumb Curl
    };
    setHandPose(lPose);
}

