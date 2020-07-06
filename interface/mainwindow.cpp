#include <stdio.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>	// For mode constants
#include <fcntl.h>	// For O_* constants
#include <atomic>
#include <errno.h>

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    printf("asdfasdf\n");

    const int shm_fd = shm_open("gaomon-s620-driver::packet", O_RDWR, 0666);
    printf("%d\n", shm_fd);
    printf("%d\n", errno);
	ftruncate(shm_fd, sizeof(std::atomic<Packet>));

	shared_packet_data = (std::atomic<Packet> *)
		mmap(0, sizeof(std::atomic<Packet>), PROT_READ, MAP_SHARED, shm_fd, 0);

    printf("%p\n", shared_packet_data);

    ui->setupUi(this);

	timerId = startTimer(5);
	ui->image_label->show();
}

MainWindow::~MainWindow() {
	killTimer(timerId);
	delete ui;
}

void MainWindow::timerEvent(QTimerEvent * event) {

    /*Packet packet = shared_packet_data->load();

	ui->x_value->setNum(packet.xcord);
	ui->y_value->setNum(packet.ycord);

	ui->pressure_bar->setValue(packet.pressure);
    ui->pressure_value->setNum(packet.pressure);*/
}
