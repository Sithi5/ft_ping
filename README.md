# ft_ping

ft_ping is a simple command-line tool for testing network connectivity using the ICMP protocol. It is written in C and runs on Unix-like operating systems.

## Features

- Sends ICMP ECHO request packets to a specified destination host.

- Receives ICMP ECHO reply packets from the destination host.

- Calculates the round-trip time (RTT) between sending and receiving packets.

- Supports both IPv4 addresses and hostnames as destinations.

- Supports the -v option for verbose output.

## Requirements

Unix-like operating system (tested on macOS and Ubuntu)
C compiler (tested with gcc)

## Options

-v: Enable verbose output.

-h: Display help message.

## Usefull definition

### ICMP

ICMP stands for Internet Control Message Protocol. It is a protocol that is used by network devices to send error messages and operational information about network conditions. ICMP packets are small packets of data that are sent over the Internet to communicate error messages, status information, or to test connectivity between two network devices.

In the context of this `ft_ping` program in C, the ICMP protocol is used to send "ping" packets to the destination host and to receive ICMP "pong" packets back in response. The program sends an ICMP ECHO request packet to the destination host and expects an ICMP ECHO reply packet to be sent back. The time it takes to send and receive these packets is used to calculate the latency of the network connection.
