# Beeper
[![Framework Badge mbed](https://img.shields.io/badge/framework-mbed-008fbe.svg)](https://os.mbed.com/)

Mbed library for simplyfication of toggling the output pin in time pattern ie.: buzzer/LED or it can be used as morse code generator

You can choose built-in tone duration or you can specify your own. All time units are in millisecods with **bit gain 16**. That means if you want to turn on the output for 16ms you would write it as 1 (32ms = 2, 64ms = 3, etc.), this technique saves the RAM because the "pattern" will be stored in 8-bit array - this has of course a limitation of maximum tone length to 4080ms.

> Please note: `Pause` is reserved "tone duration" in which no tone is played therefore the other tones can't have this duration. You can change this constant through `mbed_app.json` please see bellow.

## Example
```cpp
#include "mbed.h"
#include "Beeper.h"

Beeper beeper(PB_6);

const uint8_t oneshot[] = {Beeper::Short, Beeper::Pause, Beeper::Short};
const uint8_t pattern[] = {Beeper::Long, Beeper::Short, Beeper::Pause, Beeper::Short};

int main() {
    // this pattern will be played only once
    beeper.pattern(oneshot, sizeof(oneshot));
    // give a time to play because previous cmd is asynchronous
    ThisThread::sleep_for(5s);

    // this pattern will be played in loop until you stop it
    beeper.pattern(pattern, sizeof(pattern), true);
    ThisThread::sleep_for(20s);

    // stop the pattern
    beeper = 0;
    return 0;
}
```

## How to change `Pause` duration
Remember that the value will be multiplied by 16 as a result. Total "silence" time: (50 * 16) + (2 * 160ms) *(delay between tones)* = 1120ms
`mbed_app.json`
```json
{
  "target_overrides": {
    "*": {
      "beeper.pause": 50
    }
  }
}
```
