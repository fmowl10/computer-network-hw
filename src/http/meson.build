# make httpMessage and httpServer serperated static libraries
# httpserver have dependency to httpMessage
httpMessage_sources = files('httpMessage.c')
httpMessage_lib = static_library('httpMessage_lib', sources : httpMessage_sources, include_directories : inc)
httpMessage_dep = declare_dependency(link_with : httpMessage_lib, include_directories : inc)

# httpServer_source = files('httpServer.c')
# httpServer_lib = static_library('httpServer', httpServer_source, include_directories:inc, link_with : httpMessage_lib)
# httpServer_dep = declare_dependency(link_with : [httpServer_lib, httpMessage_lib], dependencies : httpMessage_dep, include_directories : inc)