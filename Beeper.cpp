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

#include "Beeper.h"

Beeper::Beeper(PinName pin):
    _pin(pin, false) {
}

Beeper::~Beeper() {
    if (_pattern != nullptr) {
        delete[] _pattern;
    }
}

void Beeper::pattern(const void *pattern, size_t length, bool loop) {
    _timeout.detach();
    _pattern_length = length << 1;
    _pattern_offset = 0;
    _loop = loop;

    if (_pattern != nullptr) {
        delete[] _pattern;
    }

    auto *pattern_ptr = static_cast<const uint8_t *>(pattern);
    _pattern = new uint8_t[length];
    memcpy(_pattern, pattern_ptr, length);

    toggle();
}

Beeper &Beeper::operator= (int value) {
    _timeout.detach();
    _pattern_offset = 0;
    _pattern_length = 0;
    _pin = value;

    return *this;
}

void Beeper::toggle() {
    uint32_t duration;

    if (_pattern_offset & 1) { // delay between tones
        _pin.write(0);
        duration = MBED_CONF_BEEPER_DELAY;

    } else { // tone
        if (_pattern[_pattern_offset >> 1] != Pause) {
            _pin.write(1);
        }

        duration = _pattern[_pattern_offset >> 1];
    }

    _pattern_offset++;

    if (_pattern_offset < _pattern_length) {
        _timeout.attach(callback(this, &Beeper::toggle), std::chrono::milliseconds{duration << 4});

    } else if (_loop) { // reset and play again
        _pattern_offset = 0;
        _timeout.attach(callback(this, &Beeper::toggle), std::chrono::milliseconds{MBED_CONF_BEEPER_DELAY << 4});
    }
}
