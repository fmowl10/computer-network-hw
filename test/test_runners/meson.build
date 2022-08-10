cases = {
    'TestHttpMessage': [
        'TestHttpMessage_Runner.c',
        join_paths('..', 'TestHttpMessage.c'),
    ],
    'TestHttpServer': [
        'TestHttpServer_Runner.c',
        join_paths('..', 'TestHttpServer.c'),
    ],
}

dependenciesDic = {
    'TestHttpMessage': [httpMessage_dep, unity_dep],
    'TestHttpServer': [httpServer_dep, httpMessage_dep, unity_dep],
}

foreach case : cases.keys()
    test(
        case,
        executable(
            case,
            cases.get(case),
            dependencies: dependenciesDic.get(case),
        ),
    )
endforeach
