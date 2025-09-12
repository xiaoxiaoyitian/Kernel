#ifndef     __SocketIO_H__
#define     __SocketIO_H__

#include "func.h"
#include "Message.hh"
namespace Titan{

    class SocketIO{

        public:
            SocketIO(int fd)
            :_fd(fd)
            {

            }
          int readn(char * buff, int len);
	      int readline(char * buff, int maxlen);
          int readPacket(Packet & packet);
	      int sendn(const char * buff, int len);
	      int recvPeek(char * buff, int len) const;
          
        private:
            int     _fd;
    };

}

#endif
