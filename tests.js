

/*var hub = require('./index.js').hub('se.hektorw.application');

hub.on('myo', function(myo) {
  console.log('new myo from hub');
});*/

// var test = require('./build/Release/myo.node');
var hub = require('./index.js')('se.hektorw.application');

hub.on('myo', function(myo) {
  console.log('node, myo connected');

  myo.on('orientation', function(data) {
    console.log(data);
  });
});
