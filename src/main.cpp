#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        // For mode constants
#include <fcntl.h>           // For O_* constants
#include <libusb-1.0/libusb.h>
#include <errno.h>
#include <atomic>

#include "gaomon-s620.hpp"

const char * SHARED_MEMORY_NAME = "gaomon-s620-driver::packet";

int main() {

	if (geteuid() != 0) {
		printf("Must be executed as root.\n");
		return EACCES;
	}

	// Create shared memory space
	int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);

	if (shm_fd == -1) {
		shm_unlink(SHARED_MEMORY_NAME);
		shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
	}

	ftruncate(shm_fd, sizeof(GAOMON_S620::Packet::Packet));

	auto shared_packet_data =(std::atomic<GAOMON_S620::Packet::Packet> *) mmap(
			0, sizeof(std::atomic<GAOMON_S620::Packet::Packet>),
			PROT_WRITE, MAP_SHARED,
			shm_fd, 0
		);

	GAOMON_S620::Packet::Packet * packet = new GAOMON_S620::Packet::Packet();


	GAOMON_S620::init();

	while (true) {
		const int r = GAOMON_S620::DeviceInterface::read((uint8_t *) packet);

		if (r != 0) {
			printf("Reading error: %d\n", r);
			break;
		}

		shared_packet_data->store(*packet);

		if (packet->isPencilUpdate()) {

			GAOMON_S620::UInput::moveTo(packet->getPencilX(), packet->getPencilY());
			GAOMON_S620::UInput::setPencilMode(packet->getPencilMode());
			GAOMON_S620::UInput::setPressure(packet->getPencilPressure());

		} else if (packet->isButtonUpdate()) {
			printf("Button:\n");
			printf("\nPressed Button:%x\n", packet->getPressedButton());
		}

		GAOMON_S620::UInput::sync();

		// printf("\n");
	}

	GAOMON_S620::stop();
	delete[] packet;
	
	return 0;
}
