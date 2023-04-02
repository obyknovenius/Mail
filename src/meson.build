mail_sources = [
  'main.c',
  'mail-application.c',
  'mail-window.c',
  'mail-endpoint.c'
]

mail_deps = [
  dependency('gtk4'),
  dependency('libadwaita-1'),
]

mail_sources += gnome.compile_resources('mail-resources',
  'mail.gresource.xml',
  c_name: 'mail'
)

executable('mail', mail_sources,
  dependencies: mail_deps,
       install: true,
)
