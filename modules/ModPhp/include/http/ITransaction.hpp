#ifndef __ZIA_API_ITRANSACTION_HPP__
# define __ZIA_API_ITRANSACTION_HPP__

# include "http/IRequest.hpp"
# include "http/IResponse.hpp"
# include "network/IEndPoint.hpp"

namespace   zia
{
    namespace   api
    {
        namespace   http
        {
            /**
             * @brief Provides a way to get all informations related to the
             * request and the response.
             *
             * Even if response is always available, it should not be modified
             * in RequestModifier handler.
             *
             * @note We do not provide different interfaces for the request and
             * the response handlers, to avoid code duplication. It is up to
             * the module to modify carefully the request or the response.
             */
            class   ITransaction
            {
            public:
                virtual ~ITransaction(void) { }

                /**
                 * @brief Get the request received.
                 *
                 * @return The request.
                 *
                 * @see IRequest
                 */
                virtual const IRequest &            getRequest(void) const = 0;

                /**
                 * @brief Get the response that will be sent.
                 *
                 * @return The response.
                 *
                 * @remarks This should not be used by RequestHeaderModifier
                 * and RequestBodyModifier.
                 *
                 * @see IResponse
                 * @see RequestHeaderModifier, RequestHeaderModifier, IResponse
                 */
                virtual const IResponse &           getResponse(void) const = 0;

                /**
                 * @brief Get client endpoint to know from where it is connected.
                 *
                 * @return The client connection point.
                 *
                 * @see network::IEndPoint
                 */
                virtual const network::IEndPoint &  getClientEndPoint(void) const = 0;

                /**
                 * @brief Get on what server the client is connected.
                 *
                 * @return The server connection point.
                 *
                 * @remarks This can be used to have a module adapted to different vhost
                 *
                 * @see network::IEndPoint
                 */
                virtual const network::IEndPoint &  getServerEndPoint(void) const = 0;

                /**
                 * @brief Get the request received.
                 *
                 * @return The request.
                 *
                 * @see IRequest
                 */
                virtual IRequest &                  getRequest(void) = 0;

                /**
                 * @brief Get the response that will be sent.
                 *
                 * @return The response.
                 *
                 * @remarks This should not be used by RequestHeaderModifier
                 * and RequestBodyModifier.
                 *
                 * @see IResponse
                 * @see handler::EHook
                 */
                virtual IResponse &                 getResponse(void) = 0;

                virtual network::IEndPoint &        getClientEndPoint(void) = 0;

                virtual network::IEndPoint &        getServerEndPoint(void) = 0;
            };
        };
    };
};

#endif	/* ! __ZIA_API_ITRANSACTION_HPP__ */
