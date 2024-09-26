{
  "conditions": [
    ["OS=='win'", {
      "targets": [{
        "target_name": "simple-windows-notifications",
        "dependencies": [
          "<!(node -p \"require('node-addon-api').targets\"):node_addon_api_except",
        ],
        "sources": [
          "addon.cpp",
        ],
        "msvs_settings": {
          "VCCLCompilerTool": {
            "ExceptionHandling": 1, # /EHsc,
            "RuntimeLibrary": "2", # /MD
          },
        },
      }],
    }, {
      "targets": [{
        "target_name": "noop",
        "type": "none",
      }],
    }],
  ],
}
