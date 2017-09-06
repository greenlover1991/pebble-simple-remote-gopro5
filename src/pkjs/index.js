const REQ_CAM_STATUS = 0;
const REQ_TRIGGER_SHUTTER = 1;
const REQ_STOP_SHUTTER = 2;

const ERR_UNKNOWN = -1;
const ERR_TIMEOUT = -2;

// Clay configuration
// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');
// Initialize Clay
var clay = new Clay(clayConfig);


//  APP messages
Pebble.addEventListener('ready',
  function(e) {
    console.log("PebbleKit JS READY");
    
    Pebble.sendAppMessage({ 'JS_READY': 1 });
  }
);

Pebble.addEventListener('appmessage',
  function(e) {
    console.log('App message received: ' + JSON.stringify(e));
    
    // process app requests
    var mapCodeToFn = [];
    mapCodeToFn[REQ_CAM_STATUS] = getStatus;
    mapCodeToFn[REQ_TRIGGER_SHUTTER] = triggerShutter;
    mapCodeToFn[REQ_STOP_SHUTTER] = stopShutter;
    
    var reqCode = e.payload.REQ_CODE;
    if (reqCode in mapCodeToFn) {
      mapCodeToFn[reqCode]();
    } else {
      console.log('Cannot handle app request code for PebbleKit processing, ignoring: ' + reqCode);
    }
  }
);

function sendAppMsg(reqCode, httpCode, msg, obj) {
  // FIXME needs to be a flat json
  var json = { 'REQ_CODE': reqCode, 'RESP_CODE': httpCode, 'RESP_MESSAGE': msg };
  //if (obj != undefined) {
    //json['RESP_DATA'] = obj;
  //}
  Pebble.sendAppMessage(json);
}   


/** HTTP utils */
const MAX_TIMEOUT = 5000; // 5 seconds to connect to gopro
function get(reqCode, url, cbSuccess, cbFail, cbError) {
  var req = new XMLHttpRequest();
  req.onload = function() {
    console.log(this.responseURL + " responded: " + this.statusText);
    if (this.status == 200) {
      if (cbSuccess != undefined) {
        cbSuccess(reqCode, JSON.parse(this.responseText));
        return;
      }
    } else {
      if (cbFail != undefined) {
        cbFail(reqCode, this.status, JSON.parse(this.responseText));
        return;
      }
    }
    sendAppMsg(reqCode, this.status, this.responseURL + " responded: " + this.statusText, JSON.parse(this.responseText));
  };
  req.onerror = function(e) {
    console.log("Error connecting" + this.responseURL + ". " + e);
    if (cbError != undefined) {
      cbError(reqCode, e);
    } else {
      sendAppMsg(reqCode, ERR_UNKNOWN, e);
    }
  };
  req.ontimeout = function(e) {
    console.log("(" + this.timeout + "ms) Timeout connecting" + this.responseURL + ". " + e);
    if (cbError != undefined) {
      cbError(reqCode, e);
    } else {
      sendAppMsg(reqCode, ERR_TIMEOUT, "Connection timeout: " + url + " " + this.timeout + "ms");
    }
  };
  req.timeout = MAX_TIMEOUT;
  req.open("GET", url);
  req.send(); 
}

/** GO PRO commands */
const BASE_URL = "http://10.5.5.9/gp/gpControl";
const URL_STATUS = BASE_URL + "/status";
const URL_TRIGGER_SHUTTER = BASE_URL + "/command/shutter?p=1";
const URL_TRIGGER_SHUTTER_STOP = BASE_URL + "/command/shutter?p=0";

/**
 * Returns current camera status and settings
 *  {
 *    "status": {
 *      "1":1,"2":2,"3":0,"4":0,"6":0,"8":0,"9":0,"10":0,"11":0,"13":0,"14":0,"15":0,"16":0,"17":1,"19":0,"20":0,"21":0,"22":0,"23":0,"24":0,"26":0,"27":0,"28":2,"29":"","30":"KonradHERO4Black","31":0,"32":0,"33":0,"34":4778,"35":7523,"36":432,"37":114,"38":5298,"39":114,"40":"%10%08%13%17%13%01","41":0,"42":0,"43":0,"44":0,"45":0,"46":0,"47":0,"48":1,"49":0,"54":31798784,"55":1,"56":4,"57":932443,"58":0,"59":0,"60":500,"61":2,"62":0,"63":0,"64":0,"65":0,"66":0,"67":0
 *    },
 *    "settings": {
 *      "1":0,"2":9,"3":8,"4":0,"5":0,"6":1,"7":1,"8":1,"9":0,"10":0,"11":0,"12":0,"13":1,"14":0,"15":4,"16":0,"17":0,"18":0,"19":0,"20":0,"21":1,"22":7,"23":0,"24":0,"25":0,"26":4,"27":0,"28":0,"29":5,"30":0,"31":0,"32":0,"33":0,"34":0,"35":0,"36":0,"37":0,"38":0,"39":4,"49":0,"50":1,"51":1,"52":0,"53":0,"54":1,"55":2,"56":0,"57":0,"58":1,"59":1,"60":8,"61":1,"62":700000,"63":1,"64":1,"68":0,"69":0,"70":0,"72":1,"73":0,"74":1,"75":2,"76":3,"77":0,"78":0,"79":1,"80":1,"81":0,"82":0
 *    }
 *  }
 */
function getStatus(cbSuccess, cbError, cbTimeout) {
  // FIXME need to flatten this to an array
  get(REQ_CAM_STATUS, URL_STATUS, cbSuccess, cbError, cbTimeout);
}


/**
 * Trigger shutter (photo and video)
 * Returns empty JSON
 */
function triggerShutter(cbSuccess, cbError, cbTimeout) {
  get(REQ_TRIGGER_SHUTTER, URL_TRIGGER_SHUTTER, cbSuccess, cbError, cbTimeout);
}

/**
 * Stops shutter (video and timelapse)
 * Returns empty JSON
 */
function stopShutter(cbSuccess, cbError, cbTimeout) {
  get(REQ_STOP_SHUTTER, URL_TRIGGER_SHUTTER_STOP, cbSuccess, cbError, cbTimeout);
}