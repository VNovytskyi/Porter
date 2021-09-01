# Porter
Library to guarantee data delivery

<p align="center">
  <img src="logo.PNG">
</p>

## Overview
The library was created to organize the guarantee of data delivery over the data transmission channel. The library works in sequential mode (does not support waterfalls) and has no internal FIFO buffers.
## How to use
#### Init Porter
```c
#define DATA_LENGTH 8
porter_t porter;

void user_send_callback(const uint8_t *data, const uint8_t length);
void user_recv_callback(const uint8_t *data, const uint8_t length);
void user_tx_free_callback(void);

uint8_t initResult = porter_init(&porter, DATA_LENGTH, user_send_callback, user_recv_callback, 100);
if (initResult != PORTER_OK) {
  //handle error
}

porter_set_tx_free_callback(&porter, user_tx_free_callback);
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
uint8_t data[] = {1, 2, 3, 4, 5, 6, 7, 8};
uint8_t sendResult = porter_send(&porter, data, 8);
if (sendResult != PORTER_OK) {
  //handle error
}
```

#### Deinit Porter
```c
porter_deinit(&porter);
```

## Functions description
