(function(Pebble, window) {
  var settings = {};

  Pebble.addEventListener("ready", function(e) {
    settings = window.localStorage.getItem("pebble-flipclock-settings");
    if(settings !== "") {
      var options = JSON.parse(settings);
      Pebble.sendAppMessage(options);
    }
  });

  Pebble.addEventListener("showConfiguration", function() {
    settings = window.localStorage.getItem("pebble-flipclock-settings");
    if(!settings) {
      settings = "{}";
    }
    //Pebble.openURL("http://rexmac.dev/pfc/settings.html#" + encodeURIComponent(JSON.stringify(settings)));
    Pebble.openURL("https://s3.amazonaws.com/pebble.rexmac.com/flipclock/settings-2.0.0.html#" + encodeURIComponent(JSON.stringify(settings)));
  });

  Pebble.addEventListener("webviewclosed", function(e) {
    var rt = typeof e.response,
        options = (rt === "undefined" ? {} : JSON.parse(decodeURIComponent(e.response)));
    if(Object.keys(options).length > 0) {
      window.localStorage.setItem("pebble-flipclock-settings", JSON.stringify(options));
      Pebble.sendAppMessage(options);
    }
  })
})(Pebble, window);

