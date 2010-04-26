#ifndef __ZIA_API_HANDLER_INETWORKHANDLER_HPP__
# define __ZIA_API_HANDLER_INETWORKHANDLER_HPP__

# include <ios>

# include "IHandler.hpp"
# include "network/IEndPoint.hpp"

# if _WIN32
#  include <BaseTsd.h>

typedef SSIZE_T ssize_t;
# endif

namespace   zia
{
    namespace   api
    {
        namespace   handler
        {
            /**
             * @brief Interface for handlers which redefines some basic
             * functions like @b @c accept(2), @b @c write(2), @b @c read(2),
             * @b @c close(2).
             *
             * It allows protocol implementation like ssl.
             */
            class   INetworkHandler : public IHandler
            {
            public:
                /**
                 * @brief Agregate network related functions pointers that can
                 * be "overloaded".
                 *
                 * If a handler doesn't want to overload one of these functions
                 * it should set the pointer to @c NULL.
                 */
                struct sPtrFunc
                {
                    /**
                     * @brief Accept redefinition, called when we want to
                     * accept a new connection.
                     *
                     * @param [in] server Holds the accept socket.
                     *
                     * @return Null if an error occured. Otherwise a pointer
                     * ton an IEndPoint interface. The returned pointer MUST BE
                     * allocated using the new operator, which the server can
                     * delete later.
                     *
                     * @see network::IEndPoint
                     */
                    zia::api::network::IEndPoint* (*sock_accept) (zia::api::network::IEndPoint& server);

                    /**
                     * @brief Read redefinition, called when we want to read
                     * something on the socket.
                     *
                     * @param [in] client Holds the client socket.
                     * @param [out] buffer Buffer that should be filled by the function.
                     * @param [in] len Buffer size.
                     *
                     * @return The number of bytes read, -1 on error and 0 on EOF.
                     *
                     * @see network::IEndPoint
                     */
                    ssize_t             (*sock_read)(zia::api::network::IEndPoint& client, void* buffer, size_t len);

                    /**
                     * @brief Write redefinition, called when we want to write
                     * something on the socket.
                     *
                     * @param [in] client Holds the client socket.
                     * @param [out] buffer Buffer that have to be send.
                     * @param [in] len Buffer size.
                     *
                     * @return The number of bytes written, -1 on error and 0 on EOF.
                     *
                     * @see network::IEndPoint
                     */
                    ssize_t             (*sock_write)(zia::api::network::IEndPoint& client, const void* buffer, size_t len);

                    /**
                     * @brief Close redefinition, called when we want to close the socket.
                     *
                     * @param [in] client Holds the client socket.
                     *
                     * @return 0 on success, -1 on failure.
                     */
                    int                 (*sock_close)(zia::api::network::IEndPoint& client);
                };

                virtual ~INetworkHandler(void) { }

                /**
                 * @brief Return a structure with redefined network functions
                 * pointers.
                 *
                 * @return A copy of the structure.
                 *
                 * @see sPtrFunc
                 */
                virtual sPtrFunc    getNetworkHandlers(void) = 0;
            };
        };
    };
};

#endif	/* ! __ZIA_API_HANDLER_INETWORKHANDLER_HPP__ */
