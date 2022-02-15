//
// Created by Tobias W. on 15.02.22.
//

#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_TOUCHHANDLER_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_TOUCHHANDLER_H


class TouchHandler {
public:
    TouchHandler(int dataPin_In);
    void init();
    void loopHandler();
private:
    int dataPin;
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_TOUCHHANDLER_H
