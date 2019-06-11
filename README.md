# Robot Ronny - Wheel Firmware

This repository contains firmware for the Core Module driving wheels on robot Ronny. The Core Module is controlled through the USB UART interface via AT commands.

AT commands for robot movements are:

1. Move forward:

    AT$FORWARD=<milliseconds>,<pwm(0-255)>
    OK

2. Move backward:

    AT$BACKWARD=<milliseconds>,<pwm(0-255)>
    OK

3. Move left:

    AT$LEFT=<milliseconds>,<pwm(0-255)>
    OK

4. Move backward:

    AT$RIGHT=<milliseconds>,<pwm(0-255)>
    OK

Each of the movement is finished with the `+$STOP` URC message.

All movements have ramp-up and ramp-down phase lasting 300 milliseconds.

> It is possible to extend a movement in one direction indefinitely.

Other supported AT commands are:

    AT+CLAC - Print all supported AT commands
    AT$HELP - Print help for the implemented AT commands

## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT/) - see the [LICENSE](LICENSE) file for details.

---

Made with &#x2764;&nbsp; by [**HARDWARIO s.r.o.**](https://www.hardwario.com/) in the heart of Europe.
