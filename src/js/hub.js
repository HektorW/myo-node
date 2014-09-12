
var EventEmitter = require('events').EventEmitter,
    libmyo = require('../../build/release/myo.node'),
    util = require('util'),
    myo = require('./myo.js');



function Hub(appId) {
  this.appId = appId;
  libmyo.createHub(appId);

  this.myos = {};

  this.setupConnections();
}
util.inherits(Hub, EventEmitter);



Hub.prototype.setupConnections = function() {
  libmyo.onMyoConnect(this.onMyoConnect.bind(this));
  // libmyo.onMyoPair(this.onMyoPair.bind(this));
  // libmyo.onMyoArmRecognized(this.onMyoArmRecognized.bind(this));

  // libmyo.onMyoDisconnect(this.onMyoDisconnect.bind(this));
  // libmyo.onMyoUnPair(this.onMyoUnPair.bind(this));
  // libmyo.onMyoArmLost(this.onMyoArmLost.bind(this));

  // libmyo.onMyoPose(this.onMyoPose.bind(this));
  // libmyo.onMyoOrientationData(this.onMyoOrientationData.bind(this));
  // libmyo.onMyoAccelerationData(this.onMyoAccelerationData.bind(this));
  // libmyo.onMyoGyroscopeData(this.onMyoGyroscopeData.bind(this));
  
  // libmyo.onMyoRssi(this.onMyoRssi.bind(this));
};

Hub.prototype.close = function() {
  libmyo.close(); // #not implemented
};



///////////////
// Listeners //
///////////////
Hub.prototype.onMyoConnect = function(data) {
  var myo = new Myo(data.myoId);

  this.myos[data.myoId] = myo;

  this.emit('myo', {
    myo: myo,
    timestamp: data.timestamp,
    firmwareVersion: data.firmwareVersion
  });
};

Hub.prototype.onMyoPair = function(data) {
  var myo = this.getMyo(data.myoId);
  if (myo) { myo.emit('pair', { timestamp: data.timestamp }); }
};
Hub.prototype.onMyoArmRecognized = function(data) {
  var myo = this.getMyo(data.myoId);
  if (myo) { myo.emit('arm', { timestamp: data.timestamp }); }
};

Hub.prototype.onMyoDisconnect = function(data) {
  var myo = this.getMyo(data.myoId);
  if (myo) { myo.emit('disconnect', { timestamp: data.timestamp }); }
};

Hub.prototype.onMyoUnPair = function(data) {
  var myo = this.getMyo(data.myoId);
  if (myo) { myo.emit('unpair', { timestamp: data.timestamp }); }
};

Hub.prototype.onMyoArmLost = function(data) {
  var myo = this.getMyo(data.myoId);
  if (myo) { myo.emit('armlost', { timestamp: data.timestamp }); }
};

Hub.prototype.onMyoPose = function(data) {
  var myo = this.getMyo(data.myoId);
  if (myo) {
    myo.emit('pose', {
      timestamp: data.timestamp,
      pose: data.pose
    });
  }
};

Hub.prototype.onMyoOrientationData = function(data) {
  var myo = this.getMyo(data.myoId);
  if (myo) {
    myo.emit('orientaion', {
      timestamp: data.timestamp,
      orientaion: data.orientaion
    });
  }
};
Hub.prototype.onMyoAccelerationData = function(data) {
  var myo = this.getMyo(data.myoId);
  if (myo) {
    myo.emit('acceleration', {
      timestamp: data.timestamp,
      acceleration: data.acceleration
    });
  }
};
Hub.prototype.onMyoGyroscopeData = function(data) {
  var myo = this.getMyo(data.myoId);
  if (myo) {
    myo.emit('gyroscope', {
      timestamp: data.timestamp,
      gyroscope: data.gyroscope
    });
  }
};

Hub.prototype.onMyoRssi = function(data) {
  var myo = this.getMyo(data.myoId);
  if (myo) {
    myo.emit('rssi', {
      timestamp: data.timestamp,
      rssi: data.rssi
    });
  }
};


// Private methods
Hub.prototype.getMyo = function(myoId) {
  for (var id in this.myos) {
    if (!this.hasOwnProperty(id)) continue;

    if (id === myoId) {
      return this.myos[id];
    }
  }
  return null;
};



module.exports = Hub;