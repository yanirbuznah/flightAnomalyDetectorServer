
#ifndef STANDARDIO_H
#define STANDARDIO_H

#include "commands.h"
class StandardIO: public DefaultIO{
public:
     string read() override;
     void write(string text) override;
     void write(float f) override;
     void read(float* f) override;
     ~StandardIO() override{}

};


#endif //STANDARDIO_H
