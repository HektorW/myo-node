var Hub = require('./src/js/hub.js');

module.exports = function(appId) {
  return new Hub(appId);
};