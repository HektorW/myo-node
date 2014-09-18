

/*var hub = require('./index.js').hub('se.hektorw.application');

hub.on('myo', function(myo) {
  console.log('new myo from hub');
});*/

// var test = require('./build/Release/myo.node');
var hub = require('./index.js')('se.hektorw.application');

hub.on('myo', function(myo) {
  console.log('new myo connected');

  myo.on('orientaion', function(data) {
    console.log(data);
  });
});
