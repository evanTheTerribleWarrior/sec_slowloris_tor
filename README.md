# Slow Loris with Tor
Slow loris DoS attack with Tor ports (tested in Kali Linux 2020)

This repo is simply an extra version of Slow Loris, that has the following approach:
1. With Tor installed, you edit the Tor file to indicate how many Socks ports Tor will use (in this example, I use 5 ports by default: 9050-9090)
2. Then the total sockets that you want to perform the attack with, will be split to the Tor ports and executed in paraller (through threading)

The goal of this is to experiment with having multiple IPs from your machine that perform the attack

Here are some instructions on how to run the program:
1. Install tor
`sudo apt-get install tor`

2. Then go to `/etc/tor/torrc`
Find `SocksPort` and add the ports you want, as shown below
```bash
## Tor opens a socks proxy on port 9050 by default -- even if you don't
## configure one below. Set "SocksPort 0" if you plan to run Tor only
## as a relay, and not make any local application connections yourself.
SocksPort 9050 # Default: Bind to localhost:9050 for local connections.
SocksPort 9060
SocksPort 9070
SocksPort 9080
SocksPort 9090
```
Then find `ControlPort` and add SocksPort+1
```bash
## The port on which Tor will listen for local connections from Tor
## controller applications, as documented in control-spec.txt.
ControlPort 9051
ControlPort 9061
ControlPort 9071
ControlPort 9081
ControlPort 9091
```

Save the file - you do not need to start Tor as the bash script included in the repo starts it

3. If you define different number of SOCKS ports, you need to go to `header_files/constants.h` and change:
`TOR_PORTS_NUM` and the tor_ports array

4. Complile the slow_loris.c file (and every time you do changes to it)
`gcc slow_loris.c -o slow_loris -lpthread -g`  (the flag -g is optional but will help with debugging)

5. Run the main script
`sudo ./slow_loris_tor -t <TARGET_IP> -p <TARGET_PORT> -s <TOTAL_ATTACK_SOCKETS>`

NOTE:
Currently this code is incomplete. It runs against an IP and performs the attack, but needs changes that will come in subsequent versions.
But hopefully can be something you can play with and make it awesome!


