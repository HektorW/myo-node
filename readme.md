# myo-node
A library for using the Myo in node.js.
Currently only works in windows environment :(.

## install
[Python 2.7][https://www.python.org/] and [node-gyp][https://github.com/TooTallNate/node-gyp/] is needed in order to build the project.
```
npm install -g node-gyp
```

## build
If you have multiple installations of python in your environment path you might need to specify path to python 2.7 when using node-gyp
```
node-gyp --python=/path/to/python2.7 [command]
```

First you need to configure the build using (don't forget to specify python path if needed):
```
node-gyp configure
```

After succesfully configuring with node-gyp you only have need to build it (again, python27):
```
node-gyp build
```

If everything worked properly a build folder should have been created with project files and a release folder with the files you need to import.

## usage
Now you can use it in your own application. Make sure that all dll are available for the built project.
```javascript
var myo = require('./build/release/myo.node');

myo.addListener(function(data) {
  // use myo data
});
```
API for interfacing with the myo is in progress and will be updated.