## network_sniff
This project is a simple network analyzer. It does not rely on any networking libraries, and uses only the C socket API. 

## Usage
To use this program, simply clone this repository and run

```bash
sudo ./sniff.sh <packet_num> <options>

```
Where <packet_num> is the number of packets to read, and the valid options are --rebuild or --capture-only

**We must run this program with 'sudo' because it creates a raw socket, which is a priveleged operation.**

Running sniff.sh with valid arguments will automatically compile the program if no binary exists.  
Running with --rebuild will execute the makefile even if a binary already exists.  
Running with --capture-only will omit the report, and simply generate a file containing the packet capture.  

**The program can only capture packets as fast as they hit your network adapter, which depends on your network traffic.**

## Design
The centerpiece of this project is a raw socket, implemented in C++. The raw socket will capture any packets visible to the listening adapter.
Once captured, the identifying packet data (like MAC and IP addresses, ethernet protocols, etc) are written to a file. This gives us a snapshot of our network traffic.

After the file has been generated, a Python-based data parser sorts and identifies the packets by their protocol, and generates a simple report.

## Intent
Of course, tools like Wireshark and other network analyzers already exist, and do these things better than my program ever will. This project is primarily a pursuit of deeper understanding and mastery of networking, the TCP/IP stack, and data processing.

## Goals
Rust implementation of the raw socket.
Device manufacturer identification via MAC address.

Possible packet metamorphosis utility (UDP -> TCP shaping, or similar.)
