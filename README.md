# PianoTutor+


[![License](https://img.shields.io/github/license/gabrielebaris/piano-tutor-plus.svg?style=flat-square)](https://github.com/gabrielebaris/piano-tutor-plus/blob/master/LICENSE)

This project has been developed for the course of *Component Based Software Design*, *Master of Engineering in Embedded Computing Systems* (*University of Pisa and Sant'Anna School of Advanced Studies*).

## Assignment

The aim of this project was to port the [PianoTutor](https://www.linkedin.com/pulse/musescorearduinoleds-tutorial-tommaso-cucinotta/) already developed by prof. Tommaso Cucinotta to a Raspberry Pi. 

For this ported version, it has been decided to substitute the Arduino Uno with a Raspberry Pi, developing an ALSA component and driving the LEDs strip using the available GPIO pins. A component like that allows you to run it independently from the host running MuseScore, either on the same host or on a different one (using `aseqnet` to connect ALSA ports on different nodes).

## Dependencies

The component, to control the LED strip, depends on the [rpi_ws281x](https://github.com/jgarff/rpi_ws281x), a simple and powerful library developed by [Jeremy Garff](https://github.com/jgarff). Refer to that repository for building instructions.

Also, you have to use the [MuseScore](https://github.com/tomcucinotta/MuseScore) version patched by [prof. Cucinotta](https://github.com/tomcucinotta) so to work with PianoTutor. Also in this case, refer to that repository for building it from source.

Then, you can install the other building dependencies with

```bash
$ sudo apt-get install git make g++ libasound2-dev
```

## Build

First, move into the cloned repository. The component is shipped with a [Makefile](https://github.com/gabrielebaris/piano-tutor-plus/blob/master/Makefile) so to ease the building process. Make sure to change the `LED_STRIP_LIB_FOLDER` variable inside the Makefile to point to the correct install location for the `rpi_ws281x` library.

Then, either run

```bash
$ make debug
```

or 

```bash
$ make
```

depending on if you are interested into printing debug messages or not while running the program.

## Configure

The program relies on a [configuration file](https://github.com/gabrielebaris/piano-tutor-plus/blob/master/deploy.conf) for simply configuring its behaviour. It can be named whatever you want, as long as the content follows the right syntax. This gives you a lot of flexibility for the various parameters, without the need to recompile each time the whole program (refer to [rpi_ws281x](https://github.com/jgarff/rpi_ws281x) for a list of the available GPIO pins and DMA channels).

## Run

You can easily run the program as

```bash
$ ./bin/pianotutor+ -f deploy.conf
```

Notice that the program must be up and running to connect to the MIDI port: once terminated, the port is automatically destroyed, along with all the previous made connections.

### Headless Raspberry Pi
In case you are using a headless Raspberry Pi, you need to use `aseqnet` to allow your PC/laptop running MuseScore to correctly communicate with PianoTutor+.

On the Raspberry Pi, type

```bash
$ aseqnet
```

to open a ALSA sequencer server. Then, type

```bash
$ aplaymidi -l
```

and take note of the PianoTutor+ and network sequencer ALSA ports (something like 128:0). Finally, connect them using

```bash
$ aconnect <Network port> <PianoTutor+ port>
```
On the host running MuseScore, type

```bash
$ aseqnet <raspberry ip address> <Network port>
```

to connect to the ALSA sequencer server. Then, from inside MuseScore settings, set the keyboard as MIDI input and the network sequencer as MIDI output.

### Raspberry Pi with display

Having a display connected to the Raspberry Pi, you could run MuseScore directly on it (however, I had some troubles either in compiling/running MuseScore on a Raspberry Pi 2).

Anyway, with this configuration, you simply have to run MuseScore and, from its settings, set the keyboard as MIDI input and PianoTutor+ as MIDI output.
