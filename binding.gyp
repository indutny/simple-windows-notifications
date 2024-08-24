{
  "targets": [{
    "target_name": "addon",
    "dependencies": [
      "<!(node -p \"require('node-addon-api').targets\"):node_addon_api_except",
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
      },
    },
  }],
}
