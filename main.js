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












/**
 * Issues
 * 
 * * handle multiple myos
 *  - return instance of myo on connect?
 *    + interface with this myo instance
 *    + route c-lib callbacks to correct myo instance
 *      * should be no problem
 *    + a hub should be used as center for getting myo instances
 *  - return id of myo in every callack
 *    + closer to c lib
 *    + no need to juggle myo instances in lib
 */



/**
 * preferred API
 */

var hub = require('myo-node')('com.example.application');

hub.on('myo', function(myo) {
  // Arm
  // myo.arm.right;
  // myo.arm.left;
  // myo.arm.unknown;

  // Pose
  // myo.pose.rest;
  // myo.pose.fist;
  // myo.pose.waveIn;
  // myo.pose.waveOut;
  // myo.pose.fingersSpread;
  // myo.pose.thumbToPinky;
  // myo.pose.unknow;

  // Vibration
  // myo.vibration.short
  // myo.vibration.medium
  // myo.vibration.long



  // One of [pair/connect] will not be needed?
  myo.on('pair', function(data) {
    // data.timestamp
    // data.firmwareVersion
  });
  myo.on('connect', function(data) {
    // data.timestamp
    // data.firmwareVersion
  });
  myo.on('arm', function(data) {
    // data.timestamp;
    // data.arm;
    // data.direction;
  });



  myo.on('unpair', function(data) {
    // data.timestamp
  });
  myo.on('disconnect', function(data) {
    // data.timestamp
  });
  myo.on('armlost', function(data) {
    // data.timestamp
  });




  myo.on('pose', function(data) {
    // data.timestamp
    // data.pose
  });
  myo.on('orientaion', function(data) {
    // data.timestamp
    // data.orientation = { // type quaternion
    //    x, y, z, w
    // }
  });
  myo.on('accelerometer', function(data) {
    // data.timestamp
    // data.accelerometer = {
    //    x, y, z
    // }
  });
  myo.on('gyroscope', function(data) {
    // data.timestamp
    // data.gyroscope = {
    //    x, y, z
    // }
  });

  myo.on('rss', function(data) {
    // data.timestamp
    // data.rssi
  });


  myo.vibrate(/*  length[myo.vibration.short] */);

  myo.requestRssi();
});



