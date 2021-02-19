{
  "targets":
  [
    {
      "target_name" : "bufferfromfile",
      "product_extension": "node",
      "sources" : [ "cpp/Main.cpp" ],
      "include_dirs":
      [
        "cpp",
        "<!(node -e \"require('wbasenodejscpp')\")",
        "<!(node -e \"require('wbasenodejscpp')\")/wTools/meta",
      ],
      "cflags!": [ '-fno-exceptions' ],
      "cflags_cc!": [ '-fno-exceptions' ],
      "cflags_cc":
      [
        '-std=c++1y','-O3','-Wno-tautological-undefined-compare','-Wno-null-dereference','-Fno-delete-null-pointer-checks','-fno-delete-null-pointer-checks'
      #   '-std=c++1y','-stdlib=libc++','-O3','-Wno-tautological-undefined-compare','-Wno-null-dereference','-Fno-delete-null-pointer-checks','-fno-delete-null-pointer-checks'
      ],
      "conditions":
      [
        [ 'OS=="mac"',
          {
            "xcode_settings": {

            'OTHER_CPLUSPLUSFLAGS' : [ '-std=c++1y','-stdlib=libc++','-mavx','-O3','-Wno-tautological-undefined-compare','-Wno-null-dereference','-Fno-delete-null-pointer-checks','-fno-delete-null-pointer-checks' ],
            'OTHER_LDFLAGS' : [ '-stdlib=libc++' ],
            'MACOSX_DEPLOYMENT_TARGET': '10.14',
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
            }
          }
        ]
      ]
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies":
      [
        {
            "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
            "destination": "<(module_path)"
        }
      ]
    }
  ]
}
