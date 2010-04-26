#ifndef __ZIA_API_HANDLER_IBODYHANDLER_HPP__
# define __ZIA_API_HANDLER_IBODYHANDLER_HPP__

# include <iostream>

# include "http/ITransaction.hpp"
# include "handler/IHandler.hpp"

namespace   zia
{
    namespace   api
    {
        namespace   handler
        {
            /**
             * @brief Interface for handlers which will interact with the
             * client by reading or writing to its body.
             */
            class   IBodyHandler : public IHandler
            {
            public:
                virtual ~IBodyHandler(void) { }

                /**
                 * @brief Called by the server.
                 *
                 * @param [in] transac The transation associated with streams.
                 * @param [in] is the Input stream.
                 * @param [in] os the Output stream.
                 *
                 * @return An ECode.
                 *
                 * @see ITransaction
                 * @see ECode
                 */
                virtual ECode   operator()(zia::api::http::ITransaction & transac, std::istream & is, std::ostream & os) = 0;
            };
        };
    };
};

#endif	/* ! __ZIA_API_HANDLER_IBODYHANDLER_HPP__ */
