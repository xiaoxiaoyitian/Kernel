#ifndef     __SocketIO_H__
#define     __SocketIO_H__

#include "func.h"
#include "TLV.hh"

namespace Titan{

    class SocketIO{

        public:
            SocketIO(int fd,TLV &tlv)
            :_fd(fd)
            ,_tlv(tlv)
            {

            }

            //
            TLV  & getInstanceTLV() {
                return _tlv;
            }
            int readTLV(TLV &tlv);
            int sendTLV(TLV &tlv);
        private:
            int     _fd;
            TLV     _tlv;
    };

}

#endif