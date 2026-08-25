<!-- TOC -->
* [Communication Protocol](#communication-protocol)
    * [Master](#master)
    * [Slave](#slave)
    * [Direction](#direction)
<!-- TOC -->



# Communication Protocol

## Master

Master will send a packet with the following structure

```c++
struct MasterPacket {
    uint8_t header;
    uint16_t biasVoltage;
    uint16_t inputPower;
    uint16_t outputPower;
    uint16_t temperature;
    uint8_t calibrationDone;
    uint16_t rsv[4];
    uint8_t footer;
};
```


## Slave

Slave will send a packet in case of actual interaction or timed event with the 
following structure.

```c++
struct SlavePacket {
    uint8_t header;
    uint16_t ditherAmp;
    uint16_t ditherFreq;
    uint16_t biasPoint;
    uint16_t ditherOn;
    uint8_t calibrationRequest;
    uint16_t rst[4];
    uint8_t footer;
};
```

## Direction

The `Slave` should idle the transmission line
in case of passing of a certain amount of time
for the `Master`.

