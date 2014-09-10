
var EventEmitter = require('events').EventEmitter,
    util = require('util'),
    libmyo = require('../../build/release/myo.node');


function Myo(id) {
  this.id = id;
}
util.inherits(Myo, EventEmitter);


Myo.prototype.vibrate = function(amount) {
  libmyo.vibrateMyo(this.id, amount);
};
Myo.prototype.requestRssi = function() {
  libmyo.requestRssi(this.id);
};


Myo.prototype.arm = {
  'right': 'right',
  'left': 'left',
  'unknown': 'unknown'
};
Myo.prototype.vibration = {
  'short': 300,
  'medium': 1000,
  'long': 1500
};
Myo.prototype.pose = {
  'rest': 'libmyo_pose_rest',
  'fist': 'libmyo_pose_fist',
  'waveIn': 'libmyo_pose_wave_in',
  'waveOut': 'libmyo_pose_wave_out',
  'fingersSpread': 'libmyo_pose_fingers_spread',
  'thumbToPinky': 'libmyo_pose_thumb_to_pinky',
  'unknown': 'libmyo_pose_unknown'
};



module.exports = Myo;