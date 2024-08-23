{
  "targets": [{
    "target_name": "addon",
    "include_dirs": [
      "<!(node -e \"require('node-addon-api').include\")",
    ],
    "conditions": [
      ["OS=='win'", {
        "sources": [
          "addon.cpp",
        ],
      }],
    ],
    "msvs_settings": {
      "VCCLCompilerTool": {
        "ExceptionHandling": 1, # /EHsc,
        "RuntimeLibrary": "2", # /MD
        "AdditionalOptions": [
          "/std:c++20",
        ],
      },
    },
  }],
}
