#ifndef __ZIA_API_IMODULE_HPP__
# define __ZIA_API_IMODULE_HPP__

# include <string>
# include <vector>

# include "IConfig.hpp"
# include "handler/IHandler.hpp"

/**
 * @mainpage StreamIt API Reference
 *
 * @section sec_about About
 *
 * This API is maintained by the StreamIt Team: more info at
 * <a href="http://www.ziastream.it/">www.ziastream.it</a>.
 *
 * From here you can browse the full reference on our API.
 *
 * @section see_also See Also
 *
 *   - <a href="http://code.ziastream.it/trac/api/wiki/Pr%C3%A9sentationApi">Pr&eacute;sentationApi</a>;
 *   - <a href="http://code.ziastream.it/trac/api/wiki/R%C3%A9f%C3%A9renceRapide">R&eacute;f&eacute;renceRapide</a>;
 *   - <a href="http://code.ziastream.it/trac/api/wiki/ThreadSafety">ThreadSafety</a>.
 */

namespace   zia
{
    namespace   api
    {
        /**
         * @brief It provides basic methods to identify the module and load it.
         */
        class   IModule
        {
        public:
            virtual ~IModule(void) { }

            /**
             * @brief Get the module name.
             *
             * @return The module name.
             */
            virtual const std::string &                         getName(void) const = 0;

            /**
             * @brief Get module version.
             *
             * @return The module version.
             */
            virtual const std::string &                         getVersion(void) const = 0;

            /**
             * @brief Get the module's list of hooks.
             *
             * @return A vector of IHandler.
             *
             * @see IHandler
             */
            virtual const std::vector<handler::IHandler *> &    getHandlers(void) const = 0;

            /**
             * @brief Update module configuration.
             *
             * @param [in] conf The pointer on IConfig which holds the
             * configuration.
             *
             * @return True if the configuration was successfully loaded. False
             * otherwise and the module will be unloaded.
             *
             * @see IConfig
             */
            virtual bool                                        configure(IConfig* conf = NULL) = 0;
        };

        extern "C"
        {
            /**
             * @brief Entry point of the module.
             *
             * @return The module instance.
             *
             * @see IModule
             */
#if WIN32
            __declspec(dllexport)
#endif
            zia::api::IModule* 
#if WIN32
            __cdecl
#endif 
            ziaGetModuleInstance(void);
        }
    };
};

#endif	/* ! __ZIA_API_IMODULE_HPP__ */
