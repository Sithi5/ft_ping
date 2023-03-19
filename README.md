# ft_ping

ft_ping is a simple command-line tool for testing network connectivity using the ICMP protocol. It is written in C and runs on Unix-like operating systems.

The ping program in C uses the Internet Control Message Protocol (ICMP) to send and receive packets to and from a remote host. Here's a brief overview of how the program sends packets:

- Creating a Socket: The program first creates a raw socket using the socket() system call, specifying that the protocol to be used is ICMP.

- Setting TTL: The program sets the Time-to-Live (TTL) value of the packets to be sent. This value specifies the maximum number of network hops that the packet can take before being discarded. The program sets the TTL using the setsockopt() system call, specifying the IP_TTL option.

- Preparing ICMP Packet: The program prepares an ICMP packet by creating a data structure that includes an ICMP header and data payload. The header contains fields such as the packet type (ICMP_ECHO), packet code, sequence number, and identification number.

- Computing Checksum: The program computes the Internet Checksum for the ICMP packet using a checksum function, which takes the ICMP packet as input and returns the checksum as an unsigned short value.

- Sending Packet: The program sends the ICMP packet using the sendto() system call, specifying the destination host address and port.

- Receiving Packet: The program waits for a response packet from the destination host using the recvmsg() system call. When a response is received, the program reads the ICMP header to determine the packet type and code.

- Processing Response: If the response is an ICMP_ECHOREPLY packet, the program calculates the round-trip time (RTT) by subtracting the timestamp of the original packet from the current time. If the response is an ICMP_TIMXCEED or ICMP_UNREACH packet, the program prints an error message indicating that the packet was not delivered to the destination.

- Repeat: The program repeats this process for a specified number of times or until a timeout is reached.

In summary, the ping program uses the ICMP protocol to send and receive packets to and from a remote host, and the program calculates the RTT and reports any errors that occur during the transmission of the packets.

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

## TTL (Time To Live)

The TTL (Time To Live) is a value in the IP (Internet Protocol) header of a packet that specifies the maximum number of hops (routers) that the packet can traverse before it is discarded. Each router that the packet passes through decrements the TTL value by 1, and if the TTL reaches 0, the packet is discarded and an ICMP error message is sent back to the sender.

The TTL value is used to prevent packets from circulating indefinitely in the network, and to ensure that packets are delivered efficiently and reliably to their destination. The typical TTL value for a packet is 64, but it can be adjusted based on the network topology and the characteristics of the traffic.

## Usefull function

### getaddrinfo

the getaddrinfo function resolves an FQDN (Fully Qualified Domain Name) and returns the corresponding IP address(es) for that domain name. It works with both IPv4 and IPv6 addresses, and the results are returned in a linked list of addrinfo structures.

## Resources

[socket man](https://man7.org/linux/man-pages/man2/socket.2.html)

[how_ping_works](https://www.techtarget.com/searchnetworking/definition/ping)

[checksum](https://datatracker.ietf.org/doc/html/rfc1071)

[ip_icmp.h.](https://sites.uclouvain.be/SystInfo/usr/include/netinet/ip_icmp.h.html)
