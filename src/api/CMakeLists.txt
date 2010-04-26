SET(SUBDIRECTORIES
    handler
    http
    network
   )

INSTALL(DIRECTORY ${SUBDIRECTORIES}
        DESTINATION include/streamit
        COMPONENT Includes
        FILES_MATCHING
        REGEX "[hc]pp$"
       )

SET(HEADERS
    IConfig.hpp
    IModule.hpp
   )

INSTALL(FILES ${HEADERS}
        DESTINATION include/streamit
        COMPONENT Includes
       )
