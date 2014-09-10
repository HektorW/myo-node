
var Hub = require('./hub.js');

var myolib = {
  hub: function(appId) {
    var hub = new Hub(appid);
    return hub;
  }
};

module.exports = myolib;