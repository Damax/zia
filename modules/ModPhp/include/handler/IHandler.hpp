#ifndef __ZIA_API_IHANDLER_HPP__
# define __ZIA_API_IHANDLER_HPP__

namespace   zia
{
    namespace   api
    {
        namespace   handler
        {
            /**
             * @brief Identify a hook point in the server.
             */
            enum    EHook
            {
                NetworkModifier,        /**< Hook to replace read/write/accept function. (INetworkHandler) */
                RequestHeaderModifier,  /**< Hook to modify the request's header.        (IHeaderHandler)  */
                RequestBodyModifier,    /**< Hook to modify the request's body.          (IBodyHandler)    */
                ResponseProcessor,      /**< Hook for content generation.                (IBodyHandler)    */
                ResponseHeaderModifier, /**< Hook to modify the response's header.       (IHeaderHandler)  */
                ResponseBodyModifier    /**< Hook to modify the response's body.         (IBodyHandler)    */
            };

            /**
             * @brief Identify the values that each handler can return.
             */
            enum    ECode
            {
                Ok,             /**< It works ! */
                Decline,        /**< No treatment performed */
                ClientError,    /**< Client error, module must have set the correct error code in the response headers. */
                ServerError,    /**< Server error, module must have set the correct error code in the response headers. */
                Error           /**< Error happened after data was written to the out stream */
            };

            /**
             * @brief Interface which all handlers have to respect.
             */
            class   IHandler
            {
            public:
                virtual ~IHandler(void) { }

                /**
                 * @brief Get where the hook has to be called.
                 *
                 * @return EHook which represents the location of the hook
                 * where the handler should be added.
                 */
                virtual EHook   getHook(void) const = 0;
            };
        };
    };
};

#endif	/* ! __ZIA_API_IHANDLER_HPP__ */
