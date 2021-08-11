# Development of ESP32 firmware using ESP-IDF

## Summarized description:
Develop AWS related functions for ESP32 such as OTA, shadow, JITP

## Software platform:
+ ESP-IDF 4.3
+ Microsoft VS Code
+ espressif/esp-aws-iot sdk for aws functionality (https://github.com/espressif/esp-aws-iot)

## Product overview:
Wifi connected plug with a sensor, control relay and button.
+ AP mode in ESP32 for provisioning.
+ 3 LEDs to indicate system state.

## Software versions:
More details on each step will be shared once the project is commenced.

## Version 1: Focus - Achieving all basic functionalities
+  Environment setup: Partition table. Store AWS certificates to NVS. connect to a wifi
using hardcoded SSID and password.
+ Publish/subscribe task: read certificate from NVS. Establish connection. Send a dummy data to a specific topic every 1 minute.
+ Shadow task: control relay on/off status.
+ OTA task: implemented through aws jobs. Download from s3 and update. Update success/failure to AWS

## Version 2: Focus - security of OTA
+ Implement JITP. (both firmware and AWS side processing)
+ Security for OTA:
+ Endpoint Authenticity check
+ Download Integrity - check md5sum
+ Firmware encryption - code signed firmware in s3. Decrypt firmware in ESP32
+ Rollback functionality