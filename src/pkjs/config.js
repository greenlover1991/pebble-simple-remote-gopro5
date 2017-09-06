module.exports = [
  {
    "type": "heading",
    "defaultValue": "App Configuration"
  },
  {
    "type": "text",
    "defaultValue": "Configure GoProRemote"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Shutter"
      },
      {
        "type": "toggle",
        "messageKey": "FLICK_TO_CAPTURE",
        "defaultValue": true,
        "label": "Flick Wrist to Shutter"
      },
      {
        "type": "toggle",
        "messageKey": "VIBRATE_ON_CAPTURE",
        "defaultValue": true,
        "label": "Vibrate feedback on capture"
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];