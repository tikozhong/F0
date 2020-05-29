#ifndef _USER_APP_H_
#define _USER_APP_H_

#include "Types.h"

/* �жϱ�־״̬ */
#define SOCKETn_CONNECT                  0x01    /**< unused socket */
#define SOCKETn_DISCONNECT               0x02     /**< TCP */
#define SOCKETn_RECEIVE                  0x04     /**< UDP */
#define SOCKETn_TIMEOUT                  0x08     /**< UDP */
#define SOCKETn_SENDOK                   0x10     /**< UDP */

void SOCK_DISCON(SOCKET s);

#endif
