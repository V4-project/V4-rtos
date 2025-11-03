# V4 Protocol (V4-link)

Bytecode transfer protocol for V4 RTOS, enabling over-the-air (OTA) updates and remote code loading.

## Features

- Packet-based bytecode transfer
- CRC32 error detection
- Frame synchronization
- Flow control
- Compression support (planned)
- Encryption support (planned)

## Structure

```
protocol/
├── include/
│   ├── v4_link.h        # Protocol core
│   ├── framing.h        # Packet framing
│   ├── crc.h            # CRC calculation
│   └── transfer.h       # Transfer management
├── src/
│   ├── v4_link.c
│   ├── framing.c
│   ├── crc.c
│   └── transfer.c
└── tests/
    └── test_protocol.c
```

## Building

```bash
cmake -B build -DV4_BUILD_KERNEL=ON -DV4_BUILD_PROTOCOL=ON
cmake --build build
```

## Protocol Overview

### Packet Format

```
┌─────────┬─────────┬─────────┬──────────┬─────────┐
│ SYNC(2) │ LEN(2)  │ TYPE(1) │ DATA(N)  │ CRC(4)  │
└─────────┴─────────┴─────────┴──────────┴─────────┘

SYNC: 0xAA55 (sync marker)
LEN:  Payload length (0-1024)
TYPE: Packet type
DATA: Payload
CRC:  CRC32 checksum
```

### Packet Types

- `0x01` - BYTECODE_DATA: Bytecode chunk
- `0x02` - BYTECODE_END: Transfer complete
- `0x10` - ACK: Acknowledgment
- `0x11` - NACK: Negative acknowledgment
- `0x20` - PING: Keepalive
- `0x21` - PONG: Keepalive response

## API

### Sender Side

```c
// Initialize protocol
v4_link_init(uart_write_func, uart_read_func);

// Send bytecode
uint8_t bytecode[1024];
size_t len = compile_forth_code(bytecode, sizeof(bytecode));

v4_link_send_bytecode(bytecode, len);

// Wait for acknowledgment
if (v4_link_wait_ack(1000) == 0) {
    printf("Transfer successful\n");
}
```

### Receiver Side

```c
// Initialize protocol
v4_link_init(uart_write_func, uart_read_func);

// Receive bytecode
uint8_t bytecode[1024];
size_t len;

if (v4_link_recv_bytecode(bytecode, sizeof(bytecode), &len) == 0) {
    // Load into VM
    v4_load_bytecode(bytecode, len);

    // Send acknowledgment
    v4_link_send_ack();
}
```

## Transfer Example

```
Host                    Device
 │                        │
 ├─ BYTECODE_DATA ───────>│
 │                        │
 │<────────── ACK ────────┤
 │                        │
 ├─ BYTECODE_DATA ───────>│
 │                        │
 │<────────── ACK ────────┤
 │                        │
 ├─ BYTECODE_END ────────>│
 │                        │
 │<────────── ACK ────────┤
 │                        │
```

## Error Handling

```c
int result = v4_link_send_bytecode(bytecode, len);

switch (result) {
    case V4_LINK_OK:
        printf("Success\n");
        break;
    case V4_LINK_ERR_TIMEOUT:
        printf("Timeout\n");
        break;
    case V4_LINK_ERR_CRC:
        printf("CRC error\n");
        break;
    case V4_LINK_ERR_NACK:
        printf("Device rejected transfer\n");
        break;
}
```

## Configuration

```c
#define V4_LINK_MAX_PACKET_SIZE  1024  // Max packet size
#define V4_LINK_TIMEOUT_MS       1000  // Timeout
#define V4_LINK_MAX_RETRIES      3     // Retry count
```

## Memory Footprint

- Flash: ~1.5 KB
- RAM: ~2 KB (packet buffers)

## Performance

On ESP32-C6 @ 115200 baud:

- Throughput: ~11 KB/s (effective)
- Packet overhead: ~13% (9 bytes per packet)
- Transfer time for 1KB: ~100ms

## Future Enhancements

- LZ4 compression for smaller transfers
- AES encryption for security
- Resume capability for interrupted transfers
- Multi-packet batching

## License

Dual licensed under MIT or Apache-2.0.
