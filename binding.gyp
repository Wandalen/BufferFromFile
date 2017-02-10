{
  "targets":
  [{
    "target_name" : "BufferFromFile",
    "sources" : [ "cpp/Main.cpp" ],
    "include_dirs":
    [
      "cpp",
      "cpp/wTools/meta",
    ],
    "cflags_cc":
    [
      '-std=c++1y','-stdlib=libc++','-Wno-tautological-undefined-compare','-Wno-null-dereference','-Fno-delete-null-pointer-checks','-fno-delete-null-pointer-checks'
    ],
    "conditions":
    [
      [ 'OS=="mac"',
        {
          "xcode_settings":
          {
            'OTHER_CPLUSPLUSFLAGS' : [ '-std=c++1y','-stdlib=libc++','-Wno-tautological-undefined-compare','-Wno-null-dereference','-Fno-delete-null-pointer-checks','-fno-delete-null-pointer-checks' ],
            'OTHER_LDFLAGS' : [ '-stdlib=libc++' ],
            'MACOSX_DEPLOYMENT_TARGET': '10.7'
          }
        }
      ]
    ]
  }]
}
