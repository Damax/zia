#ifndef __ZIA_API_ICONFIG_HPP__
# define __ZIA_API_ICONFIG_HPP__

# include <map>
# include <string>

namespace   zia
{
    namespace   api
    {
        /**
         * @brief Represents the interface that the configuration handler must
         * respect to interact with a module.
         */
        class   IConfig
        {
        public:
            typedef std::map < std::string, std::multimap < std::string, std::string > > ValueMap;

            virtual ~IConfig(void) { }

            /**
             * @brief Get the configuration entries.
             *
             * @return the map of section => key/value found in configuration file.
             * the first level represent the section of the configuration.
             *
             * For example:
             * <global>
             * <key>value</key>
             * </global>
             *
             * ValueMap[global] => std::multimap<string, string>
             * ValueMap[global][key] -> iterator on all value for key 'key'
             *
             */
            virtual const ValueMap &    getEntries(void) const = 0;
        };
    };
};

#endif	/* ! __ZIA_API_ICONFIG_HPP__ */
