# ft_ping

Sending ICMP ECHO packets to network hosts

# Context

The following diagram illustrates the place of the Internet Protocol
and Internet Control Message Protocol in the protocol hierarchy:

```c
 +------+ +-----+ +-----+     +-----+
 |Telnet| | FTP | | TFTP| ... | ... |
 +------+ +-----+ +-----+     +-----+
       |   |         |           |
      +-----+     +-----+     +-----+
      | TCP |     | UDP | ... | ... |
      +-----+     +-----+     +-----+
         |           |           |
      +--------------------------+----+
      |    Internet Protocol & ICMP   |
      +--------------------------+----+
                     |
        +---------------------------+
        |   Local Network Protocol  |
        +---------------------------+
            Protocol Relationships
```

# ICMP and IP Headers

ICMP messages ([RFC792](https://tools.ietf.org/html/rfc792)) are sent using the basic IP header ([RFC791](https://tools.ietf.org/html/rfc791))

```js
Basic Internet Header Format

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |Version|  IHL  |Type of Service|          Total Length         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |         Identification        |Flags|      Fragment Offset    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Time to Live |    Protocol   |         Header Checksum       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                       Source Address                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Destination Address                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

ICMP Echo or Echo Reply Message

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |     Code      |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           Identifier          |        Sequence Number        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Data ...
   +-+-+-+-+-
```

#### IP Header (```/usr/include/netinet/ip.h```)

```c
struct iphdr
{
#if defined (__LITTLE_ENDIAN_BITFIELD)
    u_int8_t             ihl:4,      // Internet header length in 32-bit words
                         version:4;  // IP version
#elif defined (__BIG_ENDIAN_BITFIELD)
    u_int8_t             version:4,
                         ihl:4;
#endif
    u_int8_t             tos;        // Type of Service
    u_int16_t            tot_len;    // Total Length
    u_int16_t            id;         // Identification, Flags
    u_int16_t            frag_off;   // Fragment Offset
    u_int8_t             ttl;        // Time to Live
    u_int8_t             protocol;   // Protocol (ICMP = 1)
    u_int16_t            check;      // Header Checksum
    u_int32_t            saddr;      // Source Address
    u_int32_t            daddr;      // Destination Address
};
```

#### ICMP Header (```/usr/include/netinet/ip_icmp.h```)

```c
struct icmphdr
{
    u_int8_t             type;       // Message type
    u_int8_t             code;       // Type sub-code
    u_int16_t            checksum;   // Checksum of the ICMP message
    union
    {
        struct
        {
            u_int16_t    id;         // Identifier to aid in matching echos
            u_int16_t    sequence;   // Sequence number to aid in matching echos
        }                echo;       // Echo datagram
        u_int32_t        gateway;
        struct
        {
            u_int16_t    __unused;   //__
            u_int16_t    mtu;
        }                frag;
    }                    un;
};
```

#### Field Description for IP Header [RFC 792](https://tools.ietf.org/html/rfc792)

| Message Field      | Description from RFC and ```man icmp```                 |
|--------------------|---------------------------------------------------------|
| Version            | ```4```                                                 |
| IHL                | Internet header length in 32-bit words between ```5``` and ```10``` (inclusive) |
| Type of Service    | ```0```                                                 |
| Total Length       | Must be the total length of IP datagram (IP header + ICMP message) in bytes |
| Identification     | ```0``` *Used in fragmentation*                         |
| Fragment Offset    | ```0``` *Used in fragmentation*                         |
| Time to Live       | Time to live in seconds; as this field is decremented at each machine in which the datagram is processed |
| Protocol           | ```1``` for ICMP                                        |
| Header Checksum    | The 16 bit one's complement of the one's complement sum of all 16 bit words in the header |
| Source Address     | The address of the gateway or host that composes the ICMP message |
| Destination Address| The address of the gateway or host to which the message should be sent |

#### Field Description for ICMP ECHO [RFC 792](https://tools.ietf.org/html/rfc792)

| Message Field  | Description from RFC                                     |
|----------------|----------------------------------------------------------|
| Type           | ```8``` for echo message, ```0``` for echo reply message |
| Code           | ```0```                                                  |
| Checksum       | 16 bit ones's complement of the one's complement sum of the ICMP message starting with the ICMP Type. For computing the checksum , the ```checksum``` field should be zero. If the total length is odd, the received data is padded with one octet of zeros for computing the checksum. |
| Identifier     | Aid in matching echos and replies, may be zero           |
| Sequence       | Aid in matching echos and replies, may be zero           |

#### ICMP Message Types (```/usr/include/netinet/ip_icmp.h```)

```c
#define ICMP_ECHOREPLY        0    /* Echo Reply               */
#define ICMP_DEST_UNREACH     3    /* Destination Unreachable  */
#define ICMP_SOURCE_QUENCH    4    /* Source Quench            */
#define ICMP_REDIRECT         5    /* Redirect (change route)  */
#define ICMP_ECHO             8    /* Echo Request             */
#define ICMP_TIME_EXCEEDED    11   /* Time Exceeded            */
#define ICMP_PARAMETERPROB    12   /* Parameter Problem        */
#define ICMP_TIMESTAMP        13   /* Timestamp Request        */
#define ICMP_TIMESTAMPREPLY   14   /* Timestamp Reply          */
#define ICMP_INFO_REQUEST     15   /* Information Request      */
#define ICMP_INFO_REPLY       16   /* Information Reply        */
#define ICMP_ADDRESS          17   /* Address Mask Request     */
#define ICMP_ADDRESSREPLY     18   /* Address Mask Reply       */
```

#### Sending an ICMP message to socket (```man icmp```)

```c
icmp -- Internet Control Message Protocol

int    socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

Non privileged ICMP
  ICMP sockets can be opened with the SOCK_DGRAM socket type without
  requiring root privileges. The synopsis is the following:

int    socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
```

Enabling non privileged ICMP sockets for root on Linux

```
sysctl -w net.ipv4.ping_group_range="0 0"
```

# Debugging ICMP

```
sudo tcpdump -vnn -e icmp[icmptype] == 8
```

# Footnotes

You might want to read this if you're doing this on MacOS : [FreeBSD socket bugs and peculiarities](http://cseweb.ucsd.edu/~braghava/notes/freebsd-sockets.txt)
