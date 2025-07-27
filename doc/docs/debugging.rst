.. SPDX-FileCopyrightText: 2025 Anna-Lena Marx
..
.. SPDX-License-Identifier: MPL-2.0

Debugging
=========

Log Output
----------

Logs are accessible via the Raspberry Pi Pico's default UART pins
with ``115200`` bauds.

.. code-block:: bash

    picocom -b 115200 /dev/tty<DEVICE>

Debugging
---------
The Raspberry Pi Pico can be debugged via it's ``SWD`` interface and
a suitable debug adapter e.g. a Blackmagic Debug Probe.

1. Connect the Blackmagic Debug Probe to the Pi Pico's SWD header.
2. Connect to a GDB session via ``west debug --runner blackmagicprobe``.
3. Set breakpoints using ``break <full/source/code/path>:<linenumber>``.
4. Step through or step in to the code via GDB commands like ``next`` or ``step in``.


See the `Zephyr Documentation <https://docs.zephyrproject.org/latest/boards/raspberrypi/rpi_pico/doc/index.html#rpi-pico-programming-and-debugging>`_
for a closer look into Raspberry Pi Pico debug options.
