var Hub = require('./hub.js');

module.exports = {
  hub: function(appId) {
    var hub = new Hub(appid);
    return hub;
  }
};