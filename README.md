# Porter
Library to guarantee data delivery

<p align="center">
  <img src="logo.PNG">
</p>

## Overview
The library was created to organize the guarantee of data delivery over the data transmission channel. The library works in sequential mode (does not support waterfalls) and has no internal FIFO buffers.
<p align="center">
  
</p>

## How to use
#### Init Porter
```c
porter_t porter;

uint8_t initResult = porter_init(&porter, 100);
if (initResult != PORTER_OK) {
  //handle error
}
```

#### In main loop call read
If there is no received data:
```c
porter_process(&porter1, NULL, 0, clock());
```
If there is received data:
```c
porter_process(&porter1, receive_data, receive_data_length, clock());
```

#### Send data
```c
uint8_t porter_tx_buff[USER_DATA_LENGTH + PORTER_SERVICE_BYTES_COUNT];
uint8_t userData[] = { 0x56, 0x12, 0x34, 0x24, 0x95, 0x56, 0x37, 0x18 };
uint8_t sendResult = porter_send(&porter, porter_tx_buff, userData, USER_DATA_LENGTH);
if (sendResult != PORTER_OK) {
  //handle error
}
```

## Functions description

## Tests
You can run the tests for Porter to make sure it works: [BaseTest_Porter](https://github.com/VNovytskyi/BaseTest_Porter)
