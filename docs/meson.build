cdata = configuration_data()
cdata.set('TOP_SRCDIR', meson.source_root())
cdata.set('TOP_BUILDDIR', meson.build_root())
cdata.set('OUTPUT_DIR', join_paths(meson.build_root(),'docs'))
cdata.set('README_PATH', join_paths(meson.source_root(),'README.md'))
cdata.set('VERSION', meson.project_version())
cdata.set('PROJECT_NAME', meson.project_name())

doxyfile = configure_file(
    input: 'Doxyfile.in',
    output: 'Doxyfile',
    configuration: cdata,
    install: false)

doxygen = find_program('doxygen')

if(not doxygen.found())
    error('doxygen not found')
endif

dot = find_program('dot')

if(not dot.found())
    error('dot not found')
endif

doc_target = custom_target(
    'docs',
    command: [doxygen, doxyfile], 
    input : [
        project_header_files, 
        project_sources
    ], 
    output : ['.'], install : false)

make = find_program('make')

if(not make.found())
    error('make not found')
endif

pdflatex = find_program('pdflatex')

if(not pdflatex.found())
    error('pdflatex not found')
endif

latex_makefile = join_paths(
    join_paths(meson.build_root(), 'docs'),
    'latex')

pdf_target = run_target(
    'pdf', 
    command: [make, '-C', latex_makefile],
    depends : [doc_target])