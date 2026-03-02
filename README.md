# Women's Safety Wearable

We know that in our day-to-day lives, women often feel unsafe in various situations. While we may not be able to eliminate the root causes due to several challenges, we aim to develop practical solutions to help women feel safer in such circumstances.

Many dangers can be mitigated if help arrives promptly. Therefore, we propose an SOS device that not only alerts a woman’s emergency contacts and the authorities in times of distress but also assists her in navigating through dark or potentially unsafe areas.

## Abstract

Our model includes three key safety features:
1. **SOS Button**: When pressed, it sends the device’s live location to your emergency contacts and the police.
2. **Loud Buzzer**: Activated by a sensor (or the SOS button), it emits high-volume sounds to attract attention in case you are in an isolated location.
3. **Safety Light**: A light that flickers to help you navigate dark areas, which can be switched on with the press of a button.

## Hardware Components
- GPS Module (for location tracking)
- GSM Module (for sending SMS alerts)
- SOS Button
- High-volume Buzzer
- High-brightness LED (Safety Light)
- Arduino/Microcontroller

## Usage
When the SOS button is pressed, the device will immediately trigger the loud buzzer and the flickering safety light. Simultaneously, it will acquire the current GPS location and send an SMS with a Google Maps link to the predefined emergency contacts.
