# Paper Weight of the Heart
An Internet of Things project that has two "paper weights" housing Arduino boards with Ethernet Sheilds that connect over UDP. When one of the paper weights is picked up it will send UDP datagrams to the other paper weight for it to display through LEDs that's its counter part has been picked up.

## Send UDP Datagrams from your Mac to the Arduino
The following material is needed to get started
1. Arduino UNO
2. Arduino Ethernet Shield
3. Ethernet cable from the Ethernet Sheild to the Arduino

Open the Arduino IDE and open sketch
`File->Examples->Ethernet->UDPSendReceiveString`

Ensure your Mac is connected to the same network that the Arduino is hard wired to.
Open a terminal on your Mac and run:
```
$ ifconfig -a                                                        [13:36:56]
en0: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
	ether 70:56:81:a9:3d:a7
	inet6 fe80::459:7da2:9d52:85b3%en0 prefixlen 64 secured scopeid 0x4
	inet 10.0.0.53 netmask 0xffffff00 broadcast 10.0.0.255
	inet6 2601:280:4700:2be2:1413:41da:681b:5346 prefixlen 64 autoconf secured
	inet6 2601:280:4700:2be2:f0c7:8ab9:cbbe:a1f prefixlen 64 autoconf temporary
	inet6 2601:280:4700:2be2::4fb2 prefixlen 64 dynamic
	nd6 options=201<PERFORMNUD,DAD>
	media: autoselect
	status: active
```
The truncated output above will show for example my Mac is in network `10.0.0.53`. Enter in the `UDPSendReceiveString` sketch another IP address I prefer something like `10.0.0.200`.

Once this is entered then verify and program this to your Arduino. The Arduino should have the left link light blinking green ever so often and the right link light solid green. Open up `Tools->Serial Monitor` and ensure the bottom right is set to 9600 Baud. This screen will display something when your Mac sends a UDP datagram to it.

Now on the Mac computer take the file `python/send.py` and enter in the IP that the Arduino was set to. First ensure you have python installed by running a `python --version`. To run the script simply type:
```
python2 send.py 10.0.0.200
```

When this is done you should see the following on your Serial Monitor:
```
Received packet of size 13
From 10.0.0.53, port 55687
Contents:
Hello, World!
```

## Next Steps
Now that an Arduino is able to receive datagrams from your Mac it's time to give your lighting effects. Inside the `loop()` function within the `if(packetSize)` block you can add code to blink and LED or other LED system you would like. Do this next to blink and LED after a datagram has been received.
