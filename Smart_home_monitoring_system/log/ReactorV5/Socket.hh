#ifndef __Socket_H__
#define __Socket_H__

#include <func.h>
namespace Titan{
    class Socket{
        public:
            Socket();
            explicit Socket(int fd):_fd(fd){}

            ~Socket();
            void shutdownWrite();

            int fd() const{
                return _fd;
            }
        private:
            int _fd;
    };
}

#endif