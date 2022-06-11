<div id="top"></div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about">About</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#components">Components</a></li>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>

<!-- ABOUT -->
## About

![Transmitter Picture][transmitter-picture]

A generic transmitter based on ESP32 \ Arduino boards using nRF24 transceiver module. This project aims to provide an extremely simple to use and build
transmitter for remotelly controlling RC vehicles: be it a drone, an RC airplane or even an RC car.
The components used in this project are the cheapest available. They are super easy to find anywhere.

This remote controller was mainly used to control __drones__ and __RC airplanes__ thus you'll hear the terms: `Throttle`, `Pitch`, `Roll` and `Yaw` a lot!
You don't need to be familiar with these terms. These are just 4 values that this remote controller sends for controlling a drone \ RC airplane.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started
This project was built using [__PlatformIO__](https://platformio.org). In order to use this project, you only need to have a basic knowledge
of how __PlatformIO__ works.
Otherwise, if you're using __ArduinoIDE__ then all you need to do is to copy the ```arduino/transmitter.ino``` into a local repository.

### Components
This project uses the following components:
- X1 __ESP32s__ as a microcontroller
- X1 __nrf24l01+ transceiver module__ as a radio-frequency transceiver module
- X2 __1K Ohm Linear Potentiometer__
- X2 __KY-023 Joystick Module__ as 2-axis analog input
- X1 __LCD display 16x2 WITH i2c Interface__ (or any other display)
- X1 __2-Way Toggle Switch__ as an ON \ OFF switch

### Prerequisites

All essential libraries are provided in the ```lib``` folder. Thus, thus there is no need to look for any prerequisites, everything
this project needs (in terms of code) is included in this project.

If you're using __ArduinoIDE__ then make sure to install the libraries existing in ```lib``` folder.

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/github_username/repo_name.git
   ```
2. Open the source code file ```src/transmitter.cpp```
3. Upload code to your microcontroller

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage
The source code file ```src/transmitter.cpp``` is well-explained and well-documented.

As the above picture shows, there are __two joysticks__ thus a total of 4 controlling values can be sent.
- __Left Joystick__: Horizontal Axis is used for `Yaw`. Vertical Axis is used for `Throttle` 
- __Right Joystick__: Horizontal Axis is used for `Roll`. Vertical Axis is used for `Pitch`
- __Top Left Potentiometer__: Used for controlling the sensitivity of both joysticks. This is useful for cases where subtle change in values is needed.
- __Top Right Potentiometer__: Used for switching between different channels (default: 3 channels). This is useful for rapidly switching between controlling different RC vehicles.
- __LCD Display__: Displays information. See source code for more details.

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- CONTRIBUTING -->
## Contributing
Any contributions are more than welcome! 🤠

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
[transmitter-picture]: images/transmitter.jpg
