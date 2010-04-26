#ifndef __ZIA_API_IENDPOINT_HPP_
# define __ZIA_API_IENDPOINT_HPP_

# if WIN32
#  include <Winsock2.h>
# endif

# include <string>

namespace   zia
{
    namespace   api
    {
        namespace   network
        {
            /**
             * @brief Represent a connection point.
             *
             */
            class   IEndPoint
            {
            public:
                typedef unsigned int    Ip;
                typedef unsigned short  Port;
# if _WIN32
                typedef SOCKET          Socket;
# else
                typedef int             Socket;
# endif

                virtual ~IEndPoint(void){}

                /**
                 * @brief Get on wich port socket is connected.
                 *
                 * @return The port number.
                 */
                virtual Port                getPort(void) const = 0;

                virtual const Ip &          getIp(void) const = 0;

                /**
                 * @brief Get the socket associated with the endpoint.
                 *
                 * @return The socket.
                 */
                virtual Socket              getSocket(void) const = 0;
            };
        };
    };
};

#endif	/* ! __ZIA_API_IENDPOINT_HPP_ */
