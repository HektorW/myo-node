

/*var hub = require('./index.js').hub('se.hektorw.application');

hub.on('myo', function(myo) {
  console.log('new myo from hub');
});*/

var test = require('./build/release/myo.node');

test.foo(function(data) {
  console.log(data);
});