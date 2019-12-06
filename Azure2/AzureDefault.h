#pragma comment(lib, "k4a.lib")
#include <k4a/k4a.h>

#include <stdio.h>
#include <stdlib.h>

int gettingStarted()
{
	uint32_t count = k4a_device_get_installed_count();
	if (count == 0)
	{
		printf("No k4a devices attached!\n");
		return 1;
	}

	// Open the first plugged in Kinect device
	k4a_device_t device = NULL;
	if (K4A_FAILED(k4a_device_open(K4A_DEVICE_DEFAULT, &device)))
	{
		printf("Failed to open k4a device!\n");
		return 1;
	}

	// Get the size of the serial number
	size_t serial_size = 0;
	k4a_device_get_serialnum(device, NULL, &serial_size);

	// Allocate memory for the serial, then acquire it
	char* serial = (char*)(malloc(serial_size));
	k4a_device_get_serialnum(device, serial, &serial_size);
	printf("Opened device: %s\n", serial);
	free(serial);

	// Configure a stream of 4096x3072 BRGA color data at 15 frames per second
	k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
	config.camera_fps = K4A_FRAMES_PER_SECOND_15;
	config.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
	config.color_resolution = K4A_COLOR_RESOLUTION_3072P;

	// Start the camera with the given configuration
	if (K4A_FAILED(k4a_device_start_cameras(device, &config)))
	{
		printf("Failed to start cameras!\n");
		k4a_device_close(device);
		return 1;
	}

	// Camera capture and application specific code would go here

	// Shut down the camera when finished with application logic
	k4a_device_stop_cameras(device);
	k4a_device_close(device);

	return 0;
}

int findOpen() {
	uint32_t device_count = k4a_device_get_installed_count();
	printf("Found %d connected devices:\n", device_count);

	//open a device, or read data from it, open a handle to device...
	k4a_device_t device = NULL;
	for (uint8_t deviceIndex = 0; deviceIndex < device_count; deviceIndex++)
	{
		if (K4A_RESULT_SUCCEEDED != k4a_device_open(deviceIndex, &device))
		{
			printf("%d: Failed to open device\n", deviceIndex);
			continue;
		}

		char* serial_number = NULL;
		size_t serial_number_length = 0;

		if (K4A_BUFFER_RESULT_TOO_SMALL != k4a_device_get_serialnum(device, NULL, &serial_number_length))
		{
			printf("%d: Failed to get serial number length\n", deviceIndex);
			k4a_device_close(device);
			device = NULL;
			continue;
		}

		serial_number = malloc(serial_number_length);
		if (serial_number == NULL)
		{
			printf("%d: Failed to allocate memory for serial number (%zu bytes)\n", deviceIndex, serial_number_length);
			k4a_device_close(device);
			device = NULL;
			continue;
		}

		if (K4A_BUFFER_RESULT_SUCCEEDED != k4a_device_get_serialnum(device, serial_number, &serial_number_length))
		{
			printf("%d: Failed to get serial number\n", deviceIndex);
			free(serial_number);
			serial_number = NULL;
			k4a_device_close(device);
			device = NULL;
			continue;
		}
		printf("%d: Device \"%s\"\n", deviceIndex, serial_number);
		free(serial_number);
	}
	//most of the time, computers only locate one device, but jic check for more like above ^
	
	k4a_device_t device = NULL;
	uint32_t device_count = k4a_device_get_installed_count();

	if (device_count != 1) {
		printf("Unexpected number of devices found!\n");
		return;
	}
	if(K4A_RESULT_SUCCEEDED != k4a_device_open(K4A_DEVICE_DEFAULT,&device)){
		printf("Default device failed to open.");
		return;

	k4a_device_stop_cameras(device);
	k4a_device_close(device);
}