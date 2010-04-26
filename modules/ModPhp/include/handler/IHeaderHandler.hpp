#ifndef __ZIA_API_HANDLER_IHEADERHANDLER_HPP__
# define __ZIA_API_HANDLER_IHEADERHANDLER_HPP__

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
             * @brief Interface for headers modifications.
             */
            class   IHeaderHandler : public IHandler
            {
            public:

                virtual ~IHeaderHandler(void) { }

                /**
                 * @brief Called by the server.
                 *
                 * @param [in] transac The transaction associated with the streams.
                 *
                 * @return an ECode.
                 *
                 * @see ITransaction
                 * @see ECode
                 */
                virtual ECode    operator()(zia::api::http::ITransaction & transac) = 0;
            };
        };
    };
};

#endif	/* ! __ZIA_API_HANDLER_IHEADERHANDLER_HPP__ */
