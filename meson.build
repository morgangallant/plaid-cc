project('plaidcc', 'cpp',
  version : '1.0.0',
  license : 'MIT')

deps = [
  dependency('protobuf'),
  dependency('libcurl')
]

inc = include_directories('include')

subdir('include')
subdir('src')

h = install_headers(headers, subdir : 'plaid')

pkg_mod = import ('pkgconfig')
pkg_mod.generate(libraries : plaidlib,
                 version : '1.0',
                 name : 'libplaidcc',
                 filebase : 'plaidcc',
                 description : 'Plaid API Client Library for C++.')