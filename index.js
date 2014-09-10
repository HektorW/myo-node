var Hub = require('./src/js/hub.js');

module.exports = {
  hub: function(appId) {
    var hub = new Hub(appId);
    return hub;
  }
};