#ifndef PRECONDITION_H
#define PRECONDITION_H

#define precondition(condition, message)                                                                               \
  if (!(condition)) {                                                                                                  \
    std::cerr << "Precondition " << #condition << " failed: " << message << std::endl;                                 \
    std::terminate();                                                                                                  \
  }

#define concat(thing1, thing2) #thing1 #thing2

#endif// PRECONDITION_H
