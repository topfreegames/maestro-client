{
  "targets": [
    {
      "target_name": "maestro",
      "sources": [ "../cpplib/src/client.cpp", 
                   "../cpplib/src/utils.cpp",
                   "addon.cpp"],
      "cflags": ["-Wall", "-fexceptions"],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "include_dirs" : ["../cpplib/include", "<!(node -e \"require('nan')\")"],
      "libraries": [
        "/usr/local/lib/librestclient-cpp.a",
        "-lcurl"
      ],
      "conditions": [ 
        [ "OS=='mac'", { 
            "xcode_settings": { 
                "OTHER_CPLUSPLUSFLAGS" : ["-std=c++11","-stdlib=libc++", "-fexceptions"], 
                "OTHER_LDFLAGS": ["-stdlib=libc++"], 
                "MACOSX_DEPLOYMENT_TARGET": "10.12" 
            },
            'libraries': [
              "/usr/local/Cellar/restclient-cpp/0.4.4/lib/librestclient-cpp.dylib"
            ]
          },
        ] 
      ] 
    }
  ]
}
