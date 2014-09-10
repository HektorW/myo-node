

var hub = require('./index.js').hub('se.hektorw.application');

hub.on('myo', function(myo) {
  console.log('new myo from hub');
});