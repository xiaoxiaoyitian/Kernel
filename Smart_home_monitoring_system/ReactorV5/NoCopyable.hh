#ifndef _NOCOPYABLE_H
#define _NOCOPYABLE_H

namespace Titan{
class NoCopyable{
protected:
    NoCopyable(){}
    ~NoCopyable(){}

    NoCopyable(const NoCopyable &)=delete;
    NoCopyable &operator=(const NoCopyable &)=delete;
};
}

#endif

