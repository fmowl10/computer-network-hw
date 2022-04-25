project(
    'computer-network-hw', 'c', version : '0.0.0'
)

inc = include_directories('include')

unity_dep = dependency('unity', fallback : ['unity', 'unity_dep'])

subdir('src')
subdir('test')

executable('client', client_source, dependencies:httpMessage_dep)
# executable('server', server_source, declare_dependency:[httpMessage_dep, httpServer_dep])

subdir('docs')