# RP2040 display

## Entities

in this project there are two entities
- slave: RP2040 mcu (this project)
- master: stm32 mcu (master)

## Overview

the master will send messages to mcu intervally which the slave
should respond with a message (its whole state as a message)

## Message format

beside length the messages have some things in common like the 
header and footer format which is 

```hexdump
FC {Actual Payload} 23
```

the `FC` is header and `23` is footer. both in hex format.
The length of the messages are fixed as below and there is no crc
, LEN, TYPE and ...

### Slave Message format

```hexdump
fc 02 58 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 23
```

### Master Message format

```hexdump
fc 00 00 00 00 00 00 0a f0 00 00 00 00 00 00 00 00 00 00 23
```

## Transport

both uses rs485 for communication which the slave pins are defined in 
`Config.h`, also there is a direction pin for transmiting and 
receiving (the io pin).

## Rules for working on this project

- committing everything before touching.
- not touching `ui/` folder

## Tasks

- [ ] receiving messages from slave and parsing it.
- [ ] updating respective placeholder in display based on parse message
- [ ] sending back the rp2040's state to master as response and start listening after it