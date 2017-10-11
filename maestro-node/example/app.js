var addon = require('./node_modules/maestro-node/build/Release/maestro.node');

var maestro_url = process.env.MAESTRO_URL || 'http://localhost:8080';
console.log('calling maestro at', maestro_url);
var maestro = addon.MaestroWorker(maestro_url);

maestro.initialize();

console.log('getting address');
var address = maestro.getAddress();
console.log('address', address);

var index = 0;
function sendStatus(index) {
  setTimeout(function() {
    if (index < 5) {
      console.log('sending status occupied');
      maestro.roomOccupied();
    } else if (index < 9) {
      console.log('sending status ready');
      maestro.roomReady();
    } else if (index < 10) {
      console.log('sending status terminating');
      maestro.roomTerminating();
    }
    index = index + 1;
    if (index < 10) {
      sendStatus(index);
    } else {
      console.log('sending status terminated');
      maestro.roomTerminated();
    }
  }, 10 * 1000);
}

sendStatus(index);
