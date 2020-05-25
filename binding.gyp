{
  "targets":
  [{
    "target_name" : "BufferFromFile",
    "sources" : [ "out/cpp/Main.cpp" ],
    "include_dirs":
    [
      "out/cpp",
      "out/cpp/wTools/meta",
    ],
    'cflags!': [ '-fno-exceptions' ],
    'cflags_cc!': [ '-fno-exceptions' ],
    "cflags_cc":
    [
      '-std=c++1y','-O3','-Wno-tautological-undefined-compare','-Wno-null-dereference','-Fno-delete-null-pointer-checks','-fno-delete-null-pointer-checks'
    #   '-std=c++1y','-stdlib=libc++','-O3','-Wno-tautological-undefined-compare','-Wno-null-dereference','-Fno-delete-null-pointer-checks','-fno-delete-null-pointer-checks'
    ],
    "conditions":
    [
      [ 'OS=="mac"',
        {
          "xcode_settings":
          {
            'OTHER_CPLUSPLUSFLAGS' : [ '-std=c++1y','-stdlib=libc++','-mavx','-O3','-Wno-tautological-undefined-compare','-Wno-null-dereference','-Fno-delete-null-pointer-checks','-fno-delete-null-pointer-checks' ],
            'OTHER_LDFLAGS' : [ '-stdlib=libc++' ],
            'MACOSX_DEPLOYMENT_TARGET': '10.10',
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'

          }
        }
      ]
    ]
  }]
}
