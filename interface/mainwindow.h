#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <atomic>

typedef struct {
    uint8_t head;
    uint8_t type: 4; // lowest nibble
    uint8_t action: 4; // highest nibble
    uint16_t xcord;
    uint16_t ycord;
    uint16_t pressure;
    uint32_t left_over;
} Packet;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int timerId;
    std::atomic<Packet> * shared_packet_data;

protected:
    void timerEvent(QTimerEvent *event);
};

#endif // MAINWINDOW_H
