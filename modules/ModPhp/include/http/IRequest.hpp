#ifndef __ZIA_API_IREQUEST_HPP__
# define __ZIA_API_IREQUEST_HPP__

# include <string>

# include "http/IHeader.hpp"

namespace   zia
{
    namespace   api
    {
        namespace   http
        {
            /**
             * @brief Interface to represent the request received.
             *
             * @see IHeader
             * @see IResponse
             */
            class   IRequest
            {
            public:
                virtual ~IRequest(void) { }

                /**
                 * @brief Get method used.
                 *
                 * @return The name of the method.
                 */
                virtual const std::string & getMethod(void) const = 0;
                virtual void                setMethod(const std::string &) = 0;

                /**
                 * @brief Get the uri requested (meaning the whole uri
                 * received from the client).
                 *
                 * @return The URI.
                 */
                virtual const std::string & getUri(void) const = 0;

                /**
                 * @brief Set the uri requested.
                 *
                 * @param [in] uri The new uri.
                 */
                virtual void                setUri(const std::string & uri) = 0;

                /**
                 * @brief Get the full path of the requested resource.
                 *
                 * @return The path.
                 */
	      virtual const std::string & getPath(void) const = 0;
	      
                /**
                 * @brief Set the full path of the requested resource.
                 *
                 * @apram [in] path The new Path.
                 */
                virtual void                setPath(const std::string & path) = 0;


                /**
                 * @brief Get the HTTP version.
                 *
                 * It has to be like: '1.1', '1.0' ...
                 */
                virtual const std::string & getVersion(void) const = 0;

                /**
                 * @brief Set the HTTP version.
                 *
                 * @param [in] v The HTTP version to set. It has to be like:
                 * 'X.X' where X is a digit.
                 */
                virtual void                setVersion(const std::string & v) = 0;

                /**
                 * @brief Get HTTP headers.
                 *
                 * @return The HTTP headers.
                 *
                 * @see IHeader
                 */
                virtual const IHeader &     getHeaders(void) const = 0;

                /**
                 * @brief Get HTTP headers.
                 *
                 * @return the HTTP headers.
                 *
                 * @see IHeader
                 */
                virtual IHeader &           getHeaders(void) = 0;

                /**
                 * @brief get the request as string
                 * @return the string which represent the request
                 */
                virtual std::string         dump() const = 0;
            };
        };
    };
};

#endif	/* ! __ZIA_API_IREQUEST_HPP__ */
