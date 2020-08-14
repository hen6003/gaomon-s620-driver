#include <stdio.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>	// For mode constants
#include <fcntl.h>	// For O_* constants
#include <errno.h>

MainWindow::MainWindow(QWidget * parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	const int shm_fd = shm_open("gaomon-s620-driver::packet", O_RDWR, 0444);
	ftruncate(shm_fd, sizeof(Packet));


	shared_packet_data = (Packet *)
		mmap(0, sizeof(Packet), PROT_READ, MAP_SHARED, shm_fd, 0);

	ui->setupUi(this);

	timerId = startTimer(5);
	ui->image_label->show();
}

MainWindow::~MainWindow() {
	killTimer(timerId);
	delete ui;
}

void MainWindow::timerEvent(QTimerEvent * event) {
	ui->x_value->setNum(shared_packet_data->xcord);
	ui->y_value->setNum(shared_packet_data->ycord);

	ui->pressure_bar->setValue(shared_packet_data->pressure);
	ui->pressure_value->setNum(shared_packet_data->pressure);
}
