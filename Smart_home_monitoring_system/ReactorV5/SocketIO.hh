#ifndef     __SocketIO_H__
#define     __SocketIO_H__

#include "func.h"
#include "TLV.hh"

namespace Titan{

    class SocketIO{

        public:
            SocketIO(int fd)
            :_fd(fd)
            {

            }
          int readn(char * buff, int len);
	      int readline(char * buff, int maxlen);
          int readTLV(TLV & tlv);
          int sendTLV(TLV &tlv);
	      int sendn(const char * buff, int len);
	      int recvPeek(char * buff, int len) const;
          
        private:
            int     _fd;
    };

}

#endif
