/*
MIT License

Copyright (c) 2020 Pavel Slama

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef BEEPER_H
#define BEEPER_H

#include "mbed.h"

class Beeper {
  public:
    enum tone_type {
        Short = MBED_CONF_BEEPER_SHORT_TONE,
        Long = MBED_CONF_BEEPER_LONG_TONE,
        Pause = MBED_CONF_BEEPER_PAUSE,
    };

    Beeper(PinName pin);
    Beeper(DigitalOut *out);
    ~Beeper(void);

    /**
     * @brief Play the pattern
     *
     * @param pattern pointer to the pattern array (time/16)
     * @param length pattern array length
     * @param loop whether to play in loop or as oneshot
     */
    void pattern(const void *pattern, size_t length, bool loop = false);

    /**
     * @brief For direct control of the pin, it also cancels the pattern played
     *
     * @param value 1 = turn on, 0 turn off
     */
    void write(int value);

    /**
     * @brief Shorthand for direct control of the pin, it also cancels the pattern played
     *
     * @param value 1 = turn on, 0 turn off
     */
    Beeper &operator= (int value) {
        write(value);
        return *this;
    }

  private:
    LowPowerTimeout _timeout;
    DigitalOut *_pin;
    volatile size_t _pattern_offset = 0;
    size_t _pattern_length = 0;
    bool _loop = false;
    uint8_t *_pattern;

    /**
     * @brief Internal callback for toggling pin
     *
     */
    void toggle();
};

#endif  // BEEPER_H
