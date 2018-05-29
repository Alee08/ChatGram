# ChatGram
Chat created between two iM871A-USB devices. The devices exchange telegrams via radio frequency. The chat is based on the wireless m-bus technology.

Project for Pervasive Systems of Sapienza.

* Author: Alessandro Trapasso

* Personal Email: ale.trapasso8@gmail.com

* Account Linkedin: www.linkedin.com/in/alessandro-trapasso

* SlideShare presentation: 

# Builds and runs on GNU/Linux:
```
make && make test
```
Binary generated: ```./build/wmbusmeters```

```
make DEBUG=true
```

Binary generated: ```./build_debug/wmbusmeters```

# Run Chat
1. Launch ChatGram on the two PCs equipped with the iM871A-USB with the command: 

```
./build_debug/wmbusmeters --debug/dev/im871a.
```


In this way we are sniffing all the telegrams sent in the nearby.

2. Send telegrams until we find the address of the two devices.

The wmbus telegram headers are not encrypted. Thus the senders address, manufacturer, control flags, device version and device type can always be read.
The control flags and the ci-field will tell you if there is data that needs to be decrypted.

3. Then restart the receive program like this:
```
./build_debug/wmbusmeters /dev/im871a MyMeter generico 00101249 ""
```
(where you replace 00101249 with the message address you received.)

The telegrams sent via chat are not encrypted. Insert the quotation marks on the last field dedicated to the AES key as in the example.

4. Now the two devices are ready to chat with each other!

# IM871A-USB detection
* It is possible to add a rule to let you easily identify your iM871a:

Create the file: ```/etc/udev/rules.d/99-usb-serial.rules``` with the content:
```
SUBSYSTEM=="tty", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", SYMLINK+="im871a",MODE="0660", GROUP="yourowngroup"
SUBSYSTEM=="tty", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001", SYMLINK+="amb8465",MODE="0660", GROUP="yourowngroup"
```
* Or whenever the device is not found, run this command from the terminal:

```
chown user:user /dev/ttyUSB0
```

Replace "user" with your username

# Documents and works on wireless bus technology
http://pages.silabs.com/rs/634-SLU-379/images/introduction-to-wireless-mbus.pdf

http://www.m-bus.com/files/w4b21021.pdf

http://www.multical.hu/WiredMBus-water.pdf

The ChatGram project is based on the Fredrik Öhrström wmbusmeters project which I thank very much for the help.

https://github.com/weetmuts/wmbusmeters

