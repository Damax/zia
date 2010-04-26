#ifndef __ZIA_API_IHEADER_HPP__
# define __ZIA_API_IHEADER_HPP__

# include <string>
# include <map>

namespace   zia
{
    namespace   api
    {
        namespace   http
        {
            /**
             * @brief Provides a convenient way to interact with HTTP headers.
             */
            class   IHeader
            {
            public:
                typedef std::string Key;
                typedef std::string Value;

                virtual ~IHeader(void) { }

                /**
                 * @brief Get value associated with a key.
                 *
                 * @param [in] k The key.
                 * @param [in,out] value Where the value will be copied.
                 *
                 * @return If the key does not exist return false, otherwise
                 * return true.
                 */
                virtual bool            getValue(const Key & k, Value & value) const = 0;

                /**
                 * @brief Set a value to a given key.
                 *
                 * @param [in] k the key.
                 * @param [in] value The value.
                 */
                virtual void            setValue(const Key & k, const Value & value) = 0;


                /**
                 * @brief Operator overloading to ease header access.
                 *
                 * @see IHeader::setValue
                 *
                 * @throw std::exception if the key does not exists.
                 */
                virtual const Value &   operator[](const Key & k) const = 0;

                /**
                 * @brief Operator overloading to ease header access.
                 *
                 * If the key does not exists, it will be created.
                 *
                 * @see IHeader::getValue
                 */
                virtual Value &         operator[](const Key & k) = 0;

                /**
                 * @brief Remove a key and its associated value.
                 *
                 * @param [in] k The key to delete.
                 */
                virtual void                deleteKey(const Key & k) = 0;
            };
        };
    };
};

#endif	/* ! __ZIA_API_IHEADER_HPP__ */
