#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <libusb-1.0/libusb.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>

#define BULK_EP_OUT 	0x81
#define BULK_EP_IN  	0x01 // ?

#define VENDOR_ID	0x256c
#define PRODUCT_ID 	0x006d


namespace GAOMON_S620 {

	namespace Resolution {
		static const uint16_t WIDTH = 33020;
		static const uint16_t HEIGHT = 20320;
		static const uint16_t MAX_PRESSURE = 8192;
	};

	namespace DeviceInterface {
		static const uint16_t PACKET_LENGTH = 12;

		extern libusb_context * ctx;
		extern struct libusb_device_handle * dev_handle;

		int32_t init();
		int32_t read(uint8_t * output);
		int32_t stop();
	};

	namespace Packet {
		enum Action: uint8_t {
			NONE = 0,
			PENCIL_UPDATE 		= 0x08,
			BUTTON_UPDATE 		= 0x0e
		};

		enum PencilMode: uint8_t {
			HOVERING 		= 0x00,
			TOUCHING 		= 0x01,
			BOTTOM_BUTTON_PRESSED	= 0x02,
			TOP_BUTTON_PRESSED 	= 0x04
		};

		class Packet {
		private:
			uint8_t head;
			uint8_t type: 4; // lowest nibble
			uint8_t action: 4; // highest nibble
			uint16_t xcord;
			uint16_t ycord;
			uint16_t pressure;
			uint32_t left_over;

		public:
			Packet();
			bool isButtonUpdate();
			bool isPencilUpdate();
			uint8_t getPressedButton();
			uint8_t getPencilMode();
			uint16_t getPencilX();
			uint16_t getPencilY();
			uint16_t getPencilPressure();
			void printPacket();
		};
	};

	namespace UInput {

		extern const char * DEVICE_NAME;
		extern int fileDescriptor;

		int32_t init();
		void stop();
		void sendEvent(uint16_t type, uint16_t code, uint32_t value);
		void moveTo(const uint16_t x, const uint16_t y);
		void setPressure(const uint16_t pressure);
		void setPencilMode(const uint8_t mode);
		void sync();
	};

	void init();
	void stop();
};


// typedef struct ScreenResolution {
// 	uint16_t height;
// 	uint16_t width;
// } ScreenResolution;
//
//
// ScreenResolution * getScreenResolution(Display * display) {
//
// 	ScreenResolution * screenResolution = nullptr;
//
// 	Screen *  screen = DefaultScreenOfDisplay(display);
//
// 	if (screen == nullptr) {
// 		printf("Failed to get default display.\n");
// 		return nullptr;
// 	}
//
// 	screenResolution = new ScreenResolution();
// 	screenResolution->height = screen->height;
// 	screenResolution->width = screen->width;
//
// 	// delete [] screen;
//
// 	return screenResolution;
// }
//
// void moveMouse(Display * display, const uint16_t x, const uint16_t y) {
//     	static int screen = XDefaultScreen(display);
//     	static Window root_window = XRootWindow(display, screen);
//
// 	XWarpPointer(display, None, root_window, 0, 0, 0, 0, x, y);
// 	XFlush(display);
// }


//
// Display * display = XOpenDisplay(NULL);
//
// if (display == nullptr) {
// 	printf("Failed to open default display.\n");
// 	return 1;
// }
//
// ScreenResolution * screenResolution = getScreenResolution(display);
//
// if (screenResolution == nullptr) {
// 	printf("Failed to get screen resolution\n");
// 	return 1;
// }
