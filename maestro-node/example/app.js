var addon = require('./../build/Release/maestro.node');

var maestro_url = process.env.MAESTRO_URL;
console.log('calling maestro at', maestro_url);
var maestro = addon.MaestroWorker(maestro_url);

maestro.initialize();

function getAddress() {
  setTimeout(function(){
  }, 1000);
}

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
    if (index < 10) {
      sendStatus(index);
    }
  }, 10 * 1000);
}

sendStatus(index);

console.log('sending status terminated');
maestro.roomTerminated();
