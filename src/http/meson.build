# make httpMessage and httpServer serperated static libraries
# httpserver have dependency to httpMessage
httpMessage_source = files('httpMessage.c')
httpMessage_lib = static_library('httpMessage', httpMessage_source, include_directories:inc)

# httpServer_source = files('httpServer.c')
# httpServer_lib = static_library('httpServer', httpServer_source, include_directories:inc, link_with : httpMessage_lib)