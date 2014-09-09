var myo = require('./build/release/myo.node');
var util = require('util');


function time() {
	return process.hrtime();
}

console.log('pre: [' + time() + ' ]');
myo.addListener(function(data) {
	util.print("\u001b[2J\u001b[0;0H");
	console.log(JSON.stringify(data, null, '  '));
});
console.log('post: [' + time() + ' ]');