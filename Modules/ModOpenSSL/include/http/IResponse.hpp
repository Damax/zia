#ifndef __ZIA_API_IRESPONSE_HPP__
# define __ZIA_API_IRESPONSE_HPP__

# include "http/IHeader.hpp"

namespace   zia
{
    namespace   api
    {
        namespace   http
        {
            /**
             * @brief Interface to represent a HTTP response. It provides
             * methods to modify the header and the response line.
             *
             * @see IHeader
             * @see IRequest
             */
            class   IResponse
            {
            public:
                typedef int StatusCode;

                virtual ~IResponse(void) { }

                /**
                 * @brief Get the status code of the response.
                 *
                 * @return The status code, 0 if it is unset.
                 *
                 * 'HTTP/1.1 200 OK' -> the '200' in this string.
                 */
                virtual StatusCode          getStatusCode(void) const = 0;

                /**
                 * @brief Get the status message.
                 *
                 * @return The status message.
                 *
                 * 'HTTP/1.1 200 OK' -> the 'OK' in this string.
                 */
                virtual const std::string & getStatusMessage(void) const = 0;

                /**
                 * @brief Get the version of HTTP in response.
                 *
                 * @return The HTTP version.
                 *
                 * 'HTTP/1.1 200 OK' -> the '1.1' in this string.
                 */
                virtual const std::string & getVersion(void) const = 0;

                /**
                 * @brief Set the status code.
                 *
                 * @param [in] code The status code to set.
                 *
                 * @see IResponse::getStatusCode
                 */
                virtual void                setStatusCode(StatusCode code) = 0;

                /**
                 * @brief Set the status message.
                 *
                 * @param [in] msg The message to set.
                 *
                 * @see IResponse::getStatusMessage
                 */
                virtual void                setStatusMessage(const std::string & msg) = 0;

                /**
                 * @brief Set the HTTP version.
                 *
                 * @param [in] v The HTTP version to set.
                 *
                 * @see IResponse::getVersion
                 */
                virtual void                setVersion(const std::string & v) = 0;

                /**
                 * @brief Get HTTP headers associated with the response.
                 *
                 * @return The HTTP headers.
                 *
                 * @see IHeader
                 */
                virtual const IHeader &           getHeaders(void) const = 0;

                /**
                 * @brief Get HTTP headers.
                 *
                 * @return the HTTP headers.
                 *
                 * @see IHeader
                 */
                virtual IHeader &           getHeaders(void) = 0;
            };
        };
    };
};

#endif	/* ! __ZIA_API_IRESPONSE_HPP__ */
