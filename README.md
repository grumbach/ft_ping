# ft_ping

Sending ICMP ECHO packets to network hosts

# Description

```js
Echo or Echo Reply Message

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

#### ICMP Header (```/usr/include/netinet/ip_icmp.h```)

```c
struct icmphdr
{
    u_int8_t             type;       // message type
    u_int8_t             code;       // type sub-code
    u_int16_t            checksum;   // ones complement checksum of struct
    union
    {
        struct
        {
            u_int16_t    id;         // identifier to aid in matching echos
            u_int16_t    sequence;   // sequence number to aid in matching echos
        }                echo;       // echo datagram
        u_int32_t        gateway;    // gateway address
        struct
        {
            u_int16_t    __unused;   //__
            u_int16_t    mtu;
        }                frag;       // path mtu discovery
    }                    un;
};
```

#### Field Description for ICMP ECHO [RFC 792](https://tools.ietf.org/html/rfc792)

| Message Field  | Description from RFC                                     |
|----------------|----------------------------------------------------------|
| Type           | ```8``` for echo message, ```0``` for echo reply message |
| Code           | ```0```                                                  |
| Checksum       | 16-bit ones's complement of the one's complement sum of the ICMP message starting with the ICMP Type. For computing the checksum , the ```checksum``` field should be zero. If the total length is odd, the received data is padded with one octet of zeros for computing the checksum. |
| Identifier     | aid in matching echos and replies, may be zero           |
| Sequence       | aid in matching echos and replies, may be zero           |

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

#### Computing the Checksum ```man in_cksum```

```c
in_cksum, in4_cksum, in6_cksum -- compute Internet checksum

uint16_t    in_cksum(struct mbuf *m, int len);

uint16_t    in4_cksum(struct mbuf *m, uint8_t nxt, int off, int len);

uint16_t    in6_cksum(struct mbuf *m, uint8_t nxt, int off, int len);

If nxt is non zero, it is assumed that m starts with an IP(v4 or v6) header,
  and that the transport layer header starts after off bytes.
  For our echo ICMP header, we leave nxt and off to zero.

The in_cksum() function is equivalent to in4_cksum(m, 0, 0, len).
```

#### Sending an ```icmphdr``` to socket (```man icmp```)

```c
icmp -- Internet Control Message Protocol

Non privileged ICMP
  ICMP sockets can be opened with the SOCK_DGRAM socket type without
  requiring root privileges. The synopsis is the following:

int    socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
```
